#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct item_node_struct
{
char name[32];
int count;
struct item_node_struct *left, *right;
};

typedef struct item_node_struct item_node;

struct tree_name_node_struct {
char treeName[32];
struct tree_name_node_struct *left, *right; 
item_node *theTree;
};

typedef struct tree_name_node_struct tree_name_node;

tree_name_node *create_name_node(char treeName[32])
{
  tree_name_node *n = malloc(sizeof(tree_name_node));
  n->left = NULL;
  n->right = NULL;
  strcpy(n->treeName, treeName);

  return n;
}

item_node *create_item_node(char name[32], int count)
{
  item_node *n = malloc(sizeof(item_node));
  n->left = NULL;
  n->right = NULL;
  n->count = count;
  strcpy(n->name, name);

  return n;
}

//Functions to get first line numbers from input
int get_num_trees(FILE *ifp)
{
  int n_trees, n_items, n_commands;
  fscanf(ifp,"%d %d %d", &n_trees, &n_items, &n_commands);
  return n_trees;
}

int get_num_items(FILE *ifp)
{
  int n_trees, n_items, n_commands;
  fscanf(ifp,"%d %d %d", &n_trees, &n_items, &n_commands);
  return n_items;
}

int get_num_commands(FILE *ifp)
{
  int n_trees, n_items, n_commands;
  fscanf(ifp,"%d %d %d", &n_trees, &n_items, &n_commands);
  return n_commands;
}

//Makes tree of trees
tree_name_node *insert(tree_name_node *root, char treeName[32])
{
  if(root == NULL)//Tree is empty
  {
    return create_name_node(treeName);
  }

  if(strcmp(treeName, root->treeName) > 0)//Right insert
  {
    root->right = insert(root->right, treeName);
  }

  else//Left insert
  {
    root->left = insert(root->left,treeName);
  }
  return root;
}

//Compares root and input root
tree_name_node *findParent(tree_name_node *root, char treeName[32])
{
 
  if(root == NULL)//Tree is empty
  {
    return NULL;
  }
  
  if(strcmp(treeName, root->treeName) == 0)//Same root
  {
    return root;
  }
  else
  {
    if(strcmp(treeName, root->treeName) > 0)//Root to right 
    {
      return findParent(root->right,treeName);
    }
    else
    {
      return findParent(root->left,treeName);//Root to left
    }
  }
}

//Finds item 
item_node *finditem(item_node *item, char itemName[32])
{

  if(item == NULL)//Tree is empty
  {
    item = "NULL";
    return item;
  }
  
  if(strcmp(itemName, item->name) == 0)//Same root
  {
    return item;
  }
  else
  {
    if(strcmp(itemName, item->name) > 0)//Root to right 
    {
      return finditem(item->right, itemName);
    }
    else
    {
      return finditem(item->left, itemName);//Root to left
    }
  }
}

//Finds count before item root
int item_before(FILE *ofp, item_node *parent, char itemName[32], int left, int right, int total_count) 
{
  if(parent == NULL)
  {
    return 0;
  }

  else{//Traverses through whole tree
    total_count++;
    item_before(ofp, parent->left, itemName, left, right, total_count);
    
    item_before(ofp, parent->right, itemName,left, right, total_count);
    
    if(strcmp(parent->name, itemName) == 0)
    {
      fprintf(ofp, "item before %s: %d\n", parent->name, total_count + 1);
      return total_count;
    }
    return total_count;
  } 
}

//Searches for item
item_node *search (FILE *ofp,tree_name_node *parent, item_node *item_item, char parentName[32], char itemName[32])
{
  if(strcmp(parent->treeName, parentName) == 0 )//Searches for tree
  {
   
    if(strcmp(item_item->name, itemName) == 0)//Searches for item
    {
      fprintf(ofp,"%d %s found in %s\n", item_item->count, itemName, parentName);
      return item_item;
    }
    else//Item not found in tree
    {
      fprintf(ofp, "%s not found in %s\n", itemName, parentName);
      
    }
  }
  else//Tree does not exist 
  {
    fprintf(ofp,"%s does not exist\n", parentName);
  }
  return item_item;
}

//Makes subtree
item_node *insert_item(item_node *root, char name[32], int count)
{
    if(root == NULL)//Tree is empty
    {
      return create_item_node(name, count);
    }
    
    if(strcmp(name, root->name) > 0)//Right insert
    {
      root->right = insert_item(root->right, name, count);
    }
    else//Left insert
    {
      root->left = insert_item(root->left,name, count);
    }
  
  return root;
}

//Scans number of trees
tree_name_node *scan_trees(FILE *ifp, int ntrees, FILE *ofp, tree_name_node *tree_name)
{
  int i;
  char str[32], s[32], t[32];
  
  fgets(str, 31, ifp);
  sscanf(str, "%s", s);
  strcpy(tree_name->treeName, s);

  return tree_name;
}

//Counts total in tree
int countTree(item_node *tree)
{
  int count = tree->count;
  if(tree->left != NULL)
  {
    count += countTree(tree->left);
  }
   if(tree->right != NULL)
  {
    count += countTree(tree->right);
  }
  return count;
}

//Deletes item
item_node *deleteItem(FILE* ofp, item_node *item, char itemName[32], tree_name_node *tree)
{
  item_node *temp;

  //Find node we want to delete
  if(item == NULL)
  {
    return NULL;
  }
  else if (strcmp(itemName, item->name) > 0)
  {
    item->right = deleteItem(ofp, item->right, itemName, tree);
  }
   else if (strcmp(itemName, item->name) < 0)
  {
    item->left = deleteItem(ofp, item->left, itemName, tree);
  }

  //Item found with 3 cases
  else if(strcmp(itemName, item->name) == 0)
  { 
    fprintf(ofp, "%s deleted from %s\n", item->name, tree->treeName);
    if((item->left == NULL) && (item->right == NULL))//No children
    {
      free(item);
      item->count = 0;
      item = NULL;
    }
    else if(item->right == NULL)//One child to left
    {
      temp = item->left;

      free(item);
      item->count = 0;
      item = temp;
    }
    else if(item->left == NULL)//One child to right
    {
      temp = item->right;
    
      free(item);
      item->count = 0;
      item = temp;
    }
    else//Two children
    {
      //Finds max of left subtree
      temp = item->left;
      while(temp->right != NULL)
      {
        temp = temp->right;
        strcpy(item->name, temp->name);
        //Deletes max of left subtree 
         item->left = deleteItem(ofp,item->left, temp->name, tree);
      } 
      item->count = 0;
    }
  } 
  return item;
}

//Finds height of right subtree
int findHeightR(item_node *root, int right)
{
  if(root->right == NULL && root->left == NULL)//No children
  {
    return right; 
  }
  else if(root->right == NULL) //One child to left
  {
    right++;
    return findHeightR(root->left, right);
    
  }
  else if(root->left == NULL) //One child to right
  {
    right++;
    return findHeightR(root->right, right);
  }
  else//Two children
  {
    right++;
    return findHeightR(root->right, right);
  }

}

//Finds height of left subtree
int findHeightL(item_node *root, int left)
{

 if(root->left == NULL && root->right == NULL)//No children
  {
    return left; 
  }
  else if(root->left == NULL) //One child to left
  {
    left++;
    return findHeightL(root->left, left);
  }
  else if(root->right == NULL) //One child to right
  {
    left++;
    return findHeightL(root->left, left);
  }
  else//Two children
  {
    left++;
    return findHeightL(root->left, left);
  }
}

//Deletes tree
void deleteTree(FILE *ofp, item_node *tree)
{
  if(tree == NULL)
  {
    return;
  }
  deleteTree(ofp,tree->left);
  deleteTree(ofp, tree->right);
  free(tree);
}

//Inorder print of trees
void printTree(FILE *ofp,tree_name_node *root)
{
  if(root == NULL)
  {
    return;
  }
  printTree(ofp, root->left);
  fprintf(ofp,"%s ", root->treeName);
  printTree(ofp, root->right);
}

//Inorder print of items
void printItem(FILE *ofp,item_node *parent)
{
  if(parent == NULL)
  {
    return;
  }
  printItem(ofp, parent->left);
  fprintf(ofp, "%s ", parent->name);
  printItem(ofp, parent->right);
}

//Inorder print of items and trees
void printItemTraversal(FILE *ofp,tree_name_node *root)
{
  if(root == NULL)
  {
    return;
  }
  printItemTraversal(ofp, root->left);

  fprintf(ofp,"===%s===\n", root->treeName);

  printItem(ofp, root->theTree);

  fprintf(ofp, "\n");

  printItemTraversal(ofp, root->right);
}

int main(void) {
  FILE *ifp;
  FILE *ofp;

  ifp = fopen("cop3502-as4-input.txt", "r");
  ofp = fopen("cop3502-as4-output-tran-trisha.txt", "w");

  int ntrees, nitems, ncommands;

  ntrees = get_num_trees(ifp);
  nitems = get_num_items(ifp);
  ncommands = get_num_commands(ifp);
  
  tree_name_node *tree_name = malloc(sizeof(32));
  tree_name_node *root = NULL;

  for(int i = 0; i < ntrees; i++) //Makes upper tree
  {  
    tree_name = scan_trees(ifp, ntrees, ofp, tree_name);
    root = insert(root, tree_name->treeName);
  }

  printTree(ofp, root);
  fprintf(ofp, "\n");

  tree_name_node *parent = NULL;

  for(int i = 0; i < nitems; i++) //Makes lower tree
  {
    int n;
    char str[32], s1[32], s2[32];

    fgets(str, 31, ifp); 
    sscanf(str, "%s %s %d", s1, s2, &n);

    parent = findParent(root, s1);

    parent->theTree = insert_item(parent->theTree, s2, n);
  }

  printItemTraversal(ofp, root);
  fprintf(ofp, "====Processing Commands====\n");
  
  int count = 0;
  int total_count;
  char command[32];

  tree_name_node *root_item = NULL;
  item_node *item = NULL;
  item_node *item_item = NULL;

  for(int j = 0; j < ncommands; j++)
  {
    char str[32], s1[32], s2[32], s3[32];
    fgets(str, 31, ifp);  
    sscanf(str, "%s %s %s", s1, s2, s3);
   
    //Searches for item or tree
    if(strcmp(s1, "search") == 0)
    {
      root_item = findParent(root, s2);

      if(root_item == NULL)//No tree found
      {
        root_item = "NULL";
      }
      else//Finds item
      {
        item_item = finditem(root_item->theTree, s3);
      }
      search(ofp,root_item, item_item, s2, s3);
    }

    int total_count = 0;
    int c = 0;
    //Counts items before
    if(strcmp(s1, "item_before") == 0)
    {
      root_item = findParent(root, s2);
      item_before(ofp,root_item->theTree, s3, 0, 0, total_count);
    }

    int left = 0;
    int right = 0;
    int difference;
    if(strcmp(s1, "height_balance") == 0)
    {
      root_item = findParent(root, s2);
      if((root_item->theTree->right == NULL && root_item->theTree->left == NULL))//Only root
      {
        fprintf(ofp, "%s: left height %d, right height %d, difference %d, balanced\n", s2, 0, 0, 0);
      }

      else if(root_item->theTree->right != NULL && root_item->theTree->left == NULL)//No child to left
      {
        left = -1;
        right = findHeightR(root_item->theTree->right, 0);
        difference = abs(right - left);

        if(difference > 1)//Unbalanced tree
        {
          fprintf(ofp, "%s: left height %d, right height %d, difference %d, not balanced\n", s2, left, right, difference);
        }
        else//Balanced tree
        {
           fprintf(ofp, "%s: left height %d, right height %d, difference %d, balanced\n", s2, left, right, difference);
        }
      }

      else if(root_item->theTree->left != NULL && root_item->theTree->right == NULL)//No child to left
      {
        right = -1;
        left = findHeightL(root_item->theTree->left, 0) ;
        difference = abs(right - left);

        if(difference > 1)//Unbalanced tree
        {
          fprintf(ofp, "%s: left height %d, right height %d, difference %d, not balanced\n", s2, left, right, difference);
        }
        else//Balanced tree
        {
          fprintf(ofp, "%s: left height %d, right height %d, difference %d, balanced\n", s2, left, right, difference);
        }
      }

      else//Two children 
      { //Finds height of left and right subtree
        left = findHeightL(root_item->theTree, 0) - 1;
        right = findHeightR(root_item->theTree, 0) - 1;
        difference = abs(right - left);

        if(difference > 1)//Unbalanced
        {
          fprintf(ofp, "%s: left height %d, right height %d, difference %d, not balanced\n", s2, left, right, difference);
        }
        else//Balanced
        {
           fprintf(ofp, "%s: left height %d, right height %d, difference %d, balanced\n", s2, left , right, difference);
        }
      }
    }

    int tree_count = 0;
    //Counts entire tree
    if(strcmp(s1, "count") == 0)
    {
      root_item = findParent(root, s2);
      tree_count = countTree(root_item->theTree);
      fprintf(ofp, "%s count %d\n", root_item->treeName, tree_count);
    }

    //Deletes item from tree
    if(strcmp(s1, "delete") == 0)
    {
      root_item = findParent(root, s2);
      item_item = finditem(root_item->theTree, s3);
      deleteItem(ofp, item_item, s3, root_item);
      free(item_item);
      item_item = NULL;
    }

    //Deletes tree
    if(strcmp(s1, "delete_tree") == 0)
    {
      root_item = findParent(root, s2);
      deleteTree(ofp, root_item->theTree);
      fprintf(ofp, "%s deleted", root_item->treeName);
    }
  }

  free(tree_name);
  fclose(ifp);
  fclose(ofp);
  return 0;
}
