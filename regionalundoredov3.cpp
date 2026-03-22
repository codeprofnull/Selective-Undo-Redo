#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

//─────────────────── CharNode ───────────────────//
struct CharNode {
    int id;
    char value;
    bool alive;

    CharNode(int i, char v) : id(i), value(v), alive(true) {}
};

//─────────────────── Command ────────────────────//
struct Command {
    string action;
    int charId;

    Command(string a = "", int id = -1) : action(a), charId(id) {}
};

//─────────────────── TreeNode ───────────────────//
struct TreeNode {
    Command cmd;
    TreeNode* parent;
    vector<TreeNode*> children;
    bool active;

    TreeNode(Command c, TreeNode* p = nullptr) : cmd(c), parent(p), active(true) {
        if (parent != nullptr) parent->children.push_back(this);
    }
};

//─────────────────── Tree ───────────────────────//
struct Tree {
    TreeNode* rootNode;
    TreeNode* currentNode;

    Tree() {
        rootNode = new TreeNode(Command());
        currentNode = rootNode;
    }
};

//─────────────────── Document ───────────────────//
struct Document {
    vector<CharNode> chars;
    int nextId = 0;

    // get position of a char by id among alive chars
    int getPosition(int id) {
        int pos = 0;
        for (auto& c : chars) {
            if (c.id == id) return pos;
            if (c.alive) pos++;
        }
        return -1;
    }

    // get char id at a visible position
    int getIdAtPosition(int pos) {
        int count = 0;
        for (auto& c : chars) {
            if (c.alive) {
                if (count == pos) return c.id;
                count++;
            }
        }
        return -1;
    }

    // get all alive char ids between visible startPos and endPos
    vector<int> getIdsInRange(int startPos, int endPos) {
        vector<int> ids;
        int count = 0;
        for (auto& c : chars) {
            if (c.alive) {
                if (count >= startPos && count <= endPos) ids.push_back(c.id);
                count++;
            }
        }
        return ids;
    }

    // print visible document
    void print() {
        for (auto& c : chars) {
            if (c.alive) cout << c.value;
        }
        cout << endl;
    }

    // get CharNode by id
    CharNode* getById(int id) {
        for (auto& c : chars) {
            if (c.id == id) return &c;
        }
        return nullptr;
    }

    // insert char at visible position, returns new char id
    int insert(int pos, char value) {
        int id = nextId++;
        int count = 0;
        auto it = chars.begin();
        for (; it != chars.end(); ++it) {
            if (it->alive) {
                if (count == pos) break;
                count++;
            }
        }
        chars.insert(it, CharNode(id, value));
        return id;
    }

    // delete char at visible position, returns char id
    int deleteAt(int pos) {
        int count = 0;
        for (auto& c : chars) {
            if (c.alive) {
                if (count == pos) {
                    c.alive = false;
                    return c.id;
                }
                count++;
            }
        }
        return -1;
    }
};

//─────────────────── Tree Helpers ───────────────//
void addNode(Tree& tree, Command cmd) {
    for (TreeNode* child : tree.currentNode->children) child->active = false;
    TreeNode* newNode = new TreeNode(cmd, tree.currentNode);
    tree.currentNode = newNode;
}

//─────────────────── Actions ────────────────────//
void insert(Document& doc, Tree& tree, char c, int pos) {
    int id = doc.insert(pos, c);
    addNode(tree, Command("insert", id));
}

void deleteChar(Document& doc, Tree& tree, int pos) {
    int id = doc.deleteAt(pos);
    addNode(tree, Command("delete", id));
}

void undo(Document& doc, Tree& tree) {
    if (tree.currentNode->parent == nullptr) return;
    Command cmd = tree.currentNode->cmd;
    CharNode* cn = doc.getById(cmd.charId);
    if (cn == nullptr) return;
    if (cmd.action == "insert") cn->alive = false;
    else cn->alive = true;
    tree.currentNode->active = false;
    tree.currentNode = tree.currentNode->parent;
}

void redo(Document& doc, Tree& tree) {
    vector<TreeNode*> activeChildren;
    for (TreeNode* child : tree.currentNode->children) {
        if (child->active) activeChildren.push_back(child);
    }
    if (activeChildren.empty()) { cout << "No active branches\n"; return; }

    cout << "Which branch?\n";
    for (int i = 0; i < activeChildren.size(); i++) {
        CharNode* cn = doc.getById(activeChildren[i]->cmd.charId);
        cout << i << ". " << activeChildren[i]->cmd.action
             << " '" << (cn ? cn->value : '?') << "'\n";
    }
    int choice; cin >> choice;

    Command cmd = activeChildren[choice]->cmd;
    CharNode* cn = doc.getById(cmd.charId);
    if (cn == nullptr) return;
    if (cmd.action == "insert") cn->alive = true;
    else cn->alive = false;
    activeChildren[choice]->active = true;
    tree.currentNode = activeChildren[choice];
}

void selectiveUndo(Document& doc, Tree& tree, int startPos, int endPos) {
    vector<int> ids = doc.getIdsInRange(startPos, endPos);

    // walk tree from current to root, undo nodes whose charId is in range
    TreeNode* current = tree.currentNode;
    vector<TreeNode*> toUndo;
    while (current->parent != nullptr) {
        if (current->active &&
            find(ids.begin(), ids.end(), current->cmd.charId) != ids.end()) {
            toUndo.push_back(current);
        }
        current = current->parent;
    }

    for (TreeNode* node : toUndo) {
        CharNode* cn = doc.getById(node->cmd.charId);
        if (cn == nullptr) continue;
        if (node->cmd.action == "insert") cn->alive = false;
        else cn->alive = true;
        node->active = false;
    }
}

//─────────────────── Main ───────────────────────//
int main() {
    Document doc;
    Tree tree;

    insert(doc, tree, 'h', 0); doc.print();
    insert(doc, tree, 'e', 1); doc.print();
    insert(doc, tree, 'l', 2); doc.print();
    insert(doc, tree, 'l', 3); doc.print();
    insert(doc, tree, 'o', 4); doc.print();

    cout << "\nSelective undo positions 1-3:\n";
    selectiveUndo(doc, tree, 1, 3);
    doc.print();

    cout << "\nInsert 'a' at position 1:\n";
    insert(doc, tree, 'a', 1);
    doc.print();

    cout << "\nUndo:\n";
    undo(doc, tree);
    doc.print();

    cout << "\nRedo:\n";
    redo(doc, tree);
    doc.print();
}
