#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
using namespace std;

//-----------------------Classes-----------------//

class Command{
    public:
    string action;
    int pos;
    char charec;

    Command(string a ="",int p = -1,char c = '\0'){
        action = a;
        charec = c;
        pos = p;
    }
};

class TreeNode{
    public:
    Command cmd;
    TreeNode* parent;
    vector<TreeNode*> children;

    TreeNode(Command c,TreeNode* p = nullptr){
        cmd = c;
        parent = p;
        if(parent!= nullptr){
            parent->children.push_back(this);
        };
    };
};

class Tree{
    public:
    TreeNode* rootNode;
    TreeNode* currentNode;

    Tree(){
        rootNode= new TreeNode(Command());
        currentNode = rootNode;
    };
};

//---------------Helper Functions--------------//


void addNode(Tree& tree,Command cmd){
    TreeNode* New = new TreeNode(cmd,tree.currentNode);
    tree.currentNode = New;    
}

vector<TreeNode*> selectRange(Tree &tree, int startPos, int endPos){
    if (startPos<=endPos){
        TreeNode* currentCheck = tree.currentNode;
        vector<TreeNode*> NodeHis;
            while(currentCheck->parent!= nullptr){
                if (currentCheck->cmd.pos<=endPos && currentCheck->cmd.pos >=startPos){
                    NodeHis.push_back(currentCheck);
                }
                currentCheck = currentCheck->parent;
            }
        return NodeHis;
    }
    return vector<TreeNode*>{};
}

//--------------Action Functions----------------//

void insert(vector<char>&doc,Tree &tree,char c,int p){
    Command cmd;
    cmd.action = "insert";
    cmd.charec = c;
    cmd.pos =p;
    addNode(tree,cmd);
    doc.insert(doc.begin()+p,c);
}

void deleteChar(vector<char>&doc,Tree &tree,int p){
    Command cmd;
    cmd.action = "delete";
    cmd.charec = doc[p];
    cmd.pos =p;
    addNode(tree,cmd);
    doc.erase(doc.begin()+p);
}

void undo(vector<char>&doc,Tree &tree){
    if (tree.currentNode->parent == nullptr){return;}
    else{
        Command cmd = tree.currentNode->cmd;
        if(cmd.action == "insert"){
            doc.erase(doc.begin()+cmd.pos);
        }
        else{
            doc.insert(doc.begin()+cmd.pos,cmd.charec);
        };
        tree.currentNode = tree.currentNode->parent;
    }   
}
void redo(vector<char>&doc,Tree &tree){
    if (tree.currentNode->children.empty()){cout<<'r';
        return;}
    else{
        cout << "Which change do you want to revert to?\n";
        for(int i = 0;i<tree.currentNode->children.size();i++){
            cout<< i << "."<< tree.currentNode->children[i]->cmd.action<<","<< tree.currentNode->children[i]->cmd.pos<<","<< tree.currentNode->children[i]->cmd.charec<<endl;
        };
        cout << endl;
        int choice;
        cin>> choice;
        
        Command cmd = tree.currentNode->children[choice]->cmd;
        if(cmd.action == "insert"){
        doc.insert(doc.begin()+cmd.pos,cmd.charec);
        }
        else{
            doc.erase(doc.begin()+cmd.pos); 
        };
        tree.currentNode = tree.currentNode->children[choice];
    };
}

void selectiveUndo(vector<char>&doc, Tree &tree, int startPos, int endPos){
    vector<TreeNode*> nodes = selectRange(tree, startPos, endPos);
    sort(nodes.begin(), nodes.end(), [](TreeNode* a, TreeNode* b){
        return a->cmd.pos > b->cmd.pos;
    });
    for(TreeNode* node : nodes){
        if(node->cmd.action == "insert"){
            doc.erase(doc.begin()+node->cmd.pos);
        } else {
            doc.insert(doc.begin()+node->cmd.pos, node->cmd.charec);
        }
    }
}


//-------------Output Functions-------------------//

void printDoc(vector<char>&doc){
    for(int i=0;i<doc.size();i++){cout<<doc.at(i);};
    cout<<endl;
}

//-------------------Main--------------------//

int main(){
        vector<char> doc;
    Tree tree;

    insert(doc,tree,'h',0); printDoc(doc);
    insert(doc,tree,'e',1); printDoc(doc);
    insert(doc,tree,'l',2); printDoc(doc);
    insert(doc,tree,'l',3); printDoc(doc);
    insert(doc,tree,'o',4); printDoc(doc);

    cout << "Selective undo positions 1-3:\n";
    selectiveUndo(doc, tree, 1, 3);
    printDoc(doc);

    cout << "Undo:\n";
    undo(doc,tree);
    printDoc(doc);

    cout << "Redo:\n";
    redo(doc,tree);
    printDoc(doc);
    }