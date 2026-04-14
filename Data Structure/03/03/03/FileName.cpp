#include <iostream>
#include <queue>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

// 定义二叉树的节点结构
struct TreeNode {
    char data;            // 节点的数据
    TreeNode* left;       // 左子树指针
    TreeNode* right;      // 右子树指针
    TreeNode(char val) : data(val), left(nullptr), right(nullptr) {}
};

// 根据先序遍历字符串递归创建二叉树
TreeNode* createTree(const string& str, int& index) {
    // 如果到达字符串末尾或遇到'#'字符，返回空节点
    if (index >= str.length() || str[index] == '#') {
        index++;
        return nullptr;
    }

    // 创建当前节点，并递归创建左子树和右子树
    TreeNode* root = new TreeNode(str[index++]);
    root->left = createTree(str, index);
    root->right = createTree(str, index);

    return root;
}

// 递归实现先序遍历
void preOrderTraversal(TreeNode* root) {
    if (root == nullptr) {
        return;
    }
    cout << root->data << " "; // 访问节点
    preOrderTraversal(root->left); // 访问左子树
    preOrderTraversal(root->right); // 访问右子树
}

// 非递归实现层次遍历
void levelOrderTraversal(TreeNode* root) {
    if (root == nullptr) {
        return;
    }

    queue<TreeNode*> q; // 创建队列
    q.push(root); // 将根节点入队

    while (!q.empty()) {
        TreeNode* node = q.front(); // 获取队首节点
        q.pop(); // 移除队首节点
        cout << node->data << " "; // 访问节点

        // 将左子节点入队
        if (node->left != nullptr) {
            q.push(node->left);
        }
        // 将右子节点入队
        if (node->right != nullptr) {
            q.push(node->right);
        }
    }
}

// 获取树的高度
int getHeight(TreeNode* root) {
    if (root == nullptr) return 0;
    return 1 + max(getHeight(root->left), getHeight(root->right));
}

// 辅助函数，用于递归生成树的文本表示
void fillTreeStructure(TreeNode* root, int depth, int pos, int offset, vector<vector<string>>& res) {
    if (root == nullptr) return;

    // 确保结果向量有足够的空间来保存当前层级
    if (depth >= res.size()) {
        res.push_back(vector<string>((1 << (res.size() + 1)) - 1, " "));
    }
    // 在适当的位置存放节点值
    res[depth][pos] = string(1, root->data);
    // 递归处理左子树和右子树
    fillTreeStructure(root->left, depth + 1, pos - offset, offset / 2, res);
    fillTreeStructure(root->right, depth + 1, pos + offset, offset / 2, res);
}

// 打印树的文本表示
void printTree(TreeNode* root) {
    int height = getHeight(root);
    int width = (1 << height) - 1;
    vector<vector<string>> res(height, vector<string>(width, " "));

    fillTreeStructure(root, 0, (width - 1) / 2, (width + 1) / 4, res);

    for (const auto& level : res) {
        for (const auto& node : level) {
            cout << node;
        }
        cout << endl;
    }
}

int main() {
    string preOrder = "ABDH###E#I##CFJ###G##"; // 输入的先序遍历字符串
    int index = 0; // 字符串索引

    // 创建二叉树
    TreeNode* root = createTree(preOrder, index);

    // 输出先序遍历结果
    cout << "Preorder Traversal: ";
    preOrderTraversal(root);
    cout << endl;

    // 输出层次遍历结果
    cout << "Level Order Traversal: ";
    levelOrderTraversal(root);
    cout << endl;

    // 输出树的文本表示
    cout << "Tree Structure:" << endl;
    printTree(root);

    return 0;
}


