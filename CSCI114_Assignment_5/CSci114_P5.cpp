/*
Saishnu Ramesh Kumar (300758706)
CSCI 114 - Assignment 5

To execute program:

 Locate the file destination and directory using cd
 ls
 g++ CSci114_P5.cpp
 ./a.exe
 This will then display the output
 The file final_size.txt will be generated in the same folder and can be opened/viewed.

 Do note to ensure the input file name saved is the same name as what is written in this program, requests-1.txt.
*/

#include <iostream>
#include <fstream>

// Defining the memory block size and the number of initial nodes
const int MEMORY_BLOCK_SIZE = 1024;
const int NUM_INITIAL_NODES = 1024;

// Node struct for the free memory blocks linked list
struct Node {
    int block_id;   // ID of the memory block
    int size;       // Size of the memory block
    Node* next;     // Pointer to the next node

    Node(int id, int s) : block_id(id), size(s), next(nullptr) {} // Constructor
};

// Node struct for the allocation linked list
struct AllocationNode {
    int request_id;     // ID of the allocation request
    int block_id;       // ID of the memory block
    int size;           // Size of the allocated memory
    AllocationNode* next;   // Pointer to the next allocation node

    AllocationNode(int req_id, int blk_id, int s) : request_id(req_id), block_id(blk_id), size(s), next(nullptr) {} // Constructor
};

// Memory Allocator Class 
class MemoryAllocator {
    private:
        Node* free_memory_list;         // Pointer to the head of the free memory list
        AllocationNode* allocation_list;    // Pointer to the head of the allocation list
    
        int next_block_id;          // ID for the next memory block

    public:
        // Constructor
        MemoryAllocator() : free_memory_list(nullptr), allocation_list(nullptr), next_block_id(1) {
            // Initialize the free memory list with initial nodes
            for (int i = 1; i <= NUM_INITIAL_NODES; i++) {
                addFreeMemoryNode(MEMORY_BLOCK_SIZE);
            }
        }

        // Destructor
        ~MemoryAllocator() {
            // Clear the free memory list and allocation list
            clearList(free_memory_list);
            clearAllocationList(allocation_list);
        }

        // Function to add a new free memory node to the list
        void addFreeMemoryNode(int size) {
            Node* newNode = new Node(next_block_id++, size); // Create a new node
            insertIntoSorted(free_memory_list, newNode);    // Insert the node into the sorted list
        }

        // Function to allocate memory for a request
        void allocateMemory(int request_id, int size) {
            // Find the best-fit memory block
            Node* current = free_memory_list;
            Node* prev = nullptr;
            Node* best_fit = nullptr;
            Node* prev_best_fit = nullptr;

            while (current) {
                if (current->size >= size && (!best_fit || current->size < best_fit->size)) {
                    prev_best_fit = prev;
                    best_fit = current;
                }
                prev = current;
                current = current->next;
            }

            // If a suitable memory block is found
            if (best_fit) {
                best_fit->size -= size; // Adjust the size of the block

                // Add the allocation to the allocation list
                addToAllocationList(request_id, best_fit->block_id, size);

                // Output allocation information
                std::cout << size << " bytes have been allocated at block " << best_fit->block_id << " for request " << request_id << std::endl;

                sortFreeMemoryList(); // Sort the free memory list after allocation
            }
            else {
                std::cout << "Cannot allocate memory for request " << request_id << std::endl;
            }
        }

        // Function to sort the free memory list
        void sortFreeMemoryList() {
            if (!free_memory_list || !free_memory_list->next) {
                return;
            }

            Node* sorted = nullptr;
            Node* current = free_memory_list;

            // Insert each node into the sorted list
            while (current) {
                Node* next = current->next;
                insertIntoSorted(sorted, current);
                current = next;
            }

            free_memory_list = sorted; // Update the head of the free memory list
        }

        // Function to insert a node into a sorted list
        void insertIntoSorted(Node*& sorted, Node* newNode) {
            if (!sorted || sorted->size >= newNode->size) {
                newNode->next = sorted;
                sorted = newNode;
            }
            else {
                Node* current = sorted;
                while (current->next && current->next->size < newNode->size) {
                    current = current->next;
                }
                newNode->next = current->next;
                current->next = newNode;
            }
        }

        // Function to release memory allocated for a request
        void releaseMemory(int request_id) {
            AllocationNode* current = allocation_list;
            AllocationNode* prev = nullptr;
            AllocationNode* released_block = nullptr;

            // Find the allocation node corresponding to the request ID
            while (current && current->request_id != request_id) {
                prev = current;
                current = current->next;
            }

            // If the allocation node is found
            if (current) {
                int size = current->size;
                int block_id = current->block_id;
                released_block = current;

                // Remove the allocation from the allocation list
                if (prev)
                    prev->next = current->next;
                else
                    allocation_list = current->next;
                delete current;

                // Find the corresponding free memory block and update its size
                Node* free_current = free_memory_list;
                while (free_current) {
                    if (free_current->block_id == block_id) {
                        free_current->size += size;
                        break;
                    }
                    free_current = free_current->next;
                }

                // Output releases the information
                std::cout << size << " bytes have been returned back to block " << block_id << " for request " << request_id << std::endl;
            }
            else {
                std::cout << "Memory not found for request " << request_id << std::endl;
            }
        }

        // Function to add an allocation node to the allocation list
        void addToAllocationList(int request_id, int block_id, int size) {
            AllocationNode* newNode = new AllocationNode(request_id, block_id, size);
            newNode->next = allocation_list;
            allocation_list = newNode;
        }

        // Function to print the sizes of free memory blocks to a file
        void printFreeMemorySizeToFile(const char* filename) {
            std::ofstream outFile(filename);
            Node* current = free_memory_list;
            while (current) {
                outFile << current->size << std::endl;
                current = current->next;
            }
            outFile.close();
        }

        // Function to clear the free memory list
        void clearList(Node* list) {
            while (list) {
                Node* temp = list;
                list = list->next;
                delete temp;
            }
        }

        // Function to clear the allocation list
        void clearAllocationList(AllocationNode* list) {
            while (list) {
                AllocationNode* temp = list;
                list = list->next;
                delete temp;
            }
        }
};

// Main Function 
int main(){
    MemoryAllocator memoryAlloc; // Create an instance of MemoryAllocator

    std::ifstream inFile("requests-1.txt"); // Open the file containing memory allocation requests
    char op;
    int request_id, size;

    // Process each request from the file
    while (inFile >> op){
        inFile >> request_id;
        if (op == 'A'){
            inFile >> size;
            memoryAlloc.allocateMemory(request_id, size); // Allocate memory if the request is 'A'
        }
        else if (op == 'R'){
            memoryAlloc.releaseMemory(request_id); // Release memory if the request is 'R'
        }
    }

    inFile.close(); // Closes the file

    // Prints the sizes of free memory blocks to a file
    memoryAlloc.printFreeMemorySizeToFile("final_size.txt");

    return 0;
}

