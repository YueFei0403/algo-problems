#include <iostream>
#include <unordered_map>
#include <string>

struct Node {
    std::string url;
    std::string content;
    Node* prev;
    Node* next;

    Node(const std::string& u, const std::string& c)
        : url(u), content(c), prev(this), next(this) {}
};


class BrowserCache {
public:
    BrowserCache(int cap) : capacity(cap), size(0), head(nullptr) {}

    std::string get(const std::string& url) {
        if (map.find(url) == map.end())
            return "MISS";

        Node* node = map[url];
        moveToFront(node);
        return node->content;   
    }

    // externally callable function
    void put(const std::string& url, const std::string& content) {
        if (map.find(url) != map.end()) {
            // update existing content
            Node* node = map[url];
            node->content = content;
            moveToFront(node);
            return;
        }

        // Evict if full
        if (size == capacity)
            evict();
        // Create new node
        Node* node = new Node(url, content);
        insertFront(node);
        map[url] = node;
    }

    void printList() {
        if (!head) {
            std::cout << "(empty)\n";
            return;
        }

        Node* cur = head;
        do {
            std::cout << cur->url;
            cur = cur->next;

            if (cur != head)
                std::cout << " -> ";
        } while (cur != head);

        std::cout << " -> (back to head)\n";
    }


private:
    int capacity;
    int size;
    Node* head; // Most recently used
    std::unordered_map<std::string, Node*> map;

    // Insert new node right after head (front)
    void insertFront(Node* node) {
        if (!head) {
            head = node;
        } else {
            Node* tail = head->prev;

            node->next = head;
            node->prev = tail;

            tail->next = node;
            head->prev = node;

            head = node;
        }
        size++;
    }

    void moveToFront(Node* node) {
        if (node == head) return;

        // Remove node
        node->prev->next = node->next;
        node->next->prev = node->prev;

        // Insert after head->prev (front)
        // least-recently-used node is pushed back to the tail
        Node* tail = head->prev;

        node->next = head;
        node->prev = tail;

        tail->next = node;
        head->prev = node;

        head = node;
    }

    void evict() {
        // circular buffer: 
        //      tail ---next---> head
        //      tail <---prev--- head
        Node* tail = head->prev;
        map.erase(tail->url);

        // Remove tail
        tail->prev->next = head;
        head->prev = tail->prev;

        // If removing last node
        if (tail == head) {
            head = nullptr;
        }
        
        delete tail;
        size--;
    }
};


int main() {
    BrowserCache cache(3);

    cache.put("google.com", "<html>Google</html>");
    cache.put("youtube.com", "<html>YouTube</html>");
    cache.put("github.com", "<html>GitHub</html>");
    
    cache.printList();  // github → youtube → google (MRU → LRU)

    std::cout << cache.get("google.com") << "\n";
    cache.printList();  // google → github → youtube

    cache.put("linkedin.com", "<html>LinkedIn</html>");
    cache.printList();  // linkedin → google → github  (youtube evicted)

    return 0;
}