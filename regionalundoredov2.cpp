#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Command{
    public:
    string action;
    int pos;
    char charec;

    Command(string a ="", int p = -1, char c = '\0'){
        action = a;
        pos = p;
        charec = c;
    }
};

class TreeNode{
    public:
    Command cmd;
    TreeNode* parent;
    vector<TreeNode*> children;
    bool active;

    TreeNode(Command c, TreeNode* p = nullptr){
        cmd = c;
        parent = p;
        active = true;
        if(parent != nullptr){
            parent->children.push_back(this);
        }
    }
};

class Tree{
    public:
    TreeNode* rootNode;
    TreeNode* currentNode;

    Tree(){
        rootNode = new TreeNode(Command());
        currentNode = rootNode;
    }
};

void addNode(Tree& tree, Command cmd){
    // mark existing children as inactive — stale branches
    for(TreeNode* child : tree.currentNode->children){
        child->active = false;
    }
    TreeNode* newNode = new TreeNode(cmd, tree.currentNode);
    tree.currentNode = newNode;
}

vector<TreeNode*> selectRange(Tree& tree, int startPos, int endPos){
    if(startPos > endPos) return vector<TreeNode*>{};
    TreeNode* current = tree.currentNode;
    vector<TreeNode*> result;
    while(current->parent != nullptr){
        if(current->active &&
           current->cmd.pos >= startPos &&
           current->cmd.pos <= endPos){
            result.push_back(current);
        }
        current = current->parent;
    }
    return result;
}

void insert(vector<char>& doc, Tree& tree, char c, int p){
    Command cmd("insert", p, c);
    addNode(tree, cmd);
    doc.insert(doc.begin() + p, c);
}

void deleteChar(vector<char>& doc, Tree& tree, int p){
    Command cmd("delete", p, doc[p]);
    addNode(tree, cmd);
    doc.erase(doc.begin() + p);
}

void undo(vector<char>& doc, Tree& tree){
    if(tree.currentNode->parent == nullptr) return;
    Command cmd = tree.currentNode->cmd;
    if(cmd.action == "insert"){
        doc.erase(doc.begin() + cmd.pos);
    } else {
        doc.insert(doc.begin() + cmd.pos, cmd.charec);
    }
    tree.currentNode->active = false;
    tree.currentNode = tree.currentNode->parent;
}

void redo(vector<char>& doc, Tree& tree){
    if(tree.currentNode->children.empty()) return;

    // only show active children
    vector<TreeNode*> activeChildren;
    for(TreeNode* child : tree.currentNode->children){
        if(child->active) activeChildren.push_back(child);
    }
    if(activeChildren.empty()){
        cout << "No active branches to redo\n";
        return;
    }

    cout << "Which branch?\n";
    for(int i = 0; i < activeChildren.size(); i++){
        cout << i << ". " << activeChildren[i]->cmd.action
             << " '" << activeChildren[i]->cmd.charec
             << "' at pos " << activeChildren[i]->cmd.pos << "\n";
    }
    int choice;
    cin >> choice;

    Command cmd = activeChildren[choice]->cmd;
    if(cmd.action == "insert"){
        doc.insert(doc.begin() + cmd.pos, cmd.charec);
    } else {
        doc.erase(doc.begin() + cmd.pos);
    }
    activeChildren[choice]->active = true;
    tree.currentNode = activeChildren[choice];
}

void selectiveUndo(vector<char>& doc, Tree& tree, int startPos, int endPos){
    vector<TreeNode*> nodes = selectRange(tree, startPos, endPos);
    sort(nodes.begin(), nodes.end(), [](TreeNode* a, TreeNode* b){
        return a->cmd.pos > b->cmd.pos;
    });
    for(TreeNode* node : nodes){
        if(node->cmd.action == "insert"){
            doc.erase(doc.begin() + node->cmd.pos);
        } else {
            doc.insert(doc.begin() + node->cmd.pos, node->cmd.charec);
        }
        node->active = false;
    }
}

void printDoc(vector<char>& doc){
    for(int i = 0; i < doc.size(); i++) cout << doc.at(i);
    cout << endl;
}

int main(){
    vector<char> doc;
    Tree tree;

    insert(doc, tree, 'h', 0); printDoc(doc);
    insert(doc, tree, 'e', 1); printDoc(doc);
    insert(doc, tree, 'l', 2); printDoc(doc);
    insert(doc, tree, 'l', 3); printDoc(doc);
    insert(doc, tree, 'o', 4); printDoc(doc);

    cout << "\nSelective undo positions 1-3:\n";
    selectiveUndo(doc, tree, 1, 3);
    printDoc(doc);

    cout << "\nUndo:\n";
    undo(doc, tree); printDoc(doc);

    cout << "\nInsert 'a' at 3 (new branch):\n";
    insert(doc, tree, 'a', 3); printDoc(doc);

    cout << "\nUndo:\n";
    undo(doc, tree); printDoc(doc);

    cout << "\nRedo:\n";
    redo(doc, tree); printDoc(doc);
}
