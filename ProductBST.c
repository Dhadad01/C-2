//
// Created by naqni on 06/11/2022.
//
#include "ProductBST.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define MAX_SIZE_LINE 1024
#define INVALID_NAME "invalid_name"
#define INVALID_INPUT "invalid input"
#define INVALID_FILE "The given file is invalid.\n"
#define EQUAL 0
int name_aloc (char *name, char *dest);
Node *get_input (char *line, Node *root);
Node *node_aloc (char *name, int quantity);
void swap (Node *p1, Node *p2);
Node *delete_product_helper (Node *root, char *name, Node *real_root,int kids);
Node *add_product_helper (Node *root, char *name, int quantity, Node
*real_root);
void del_without_kids (Node *root, char *name);
Node *find_min (Node *root);
void free_Node (Node *node);

Node *node_aloc (char *name, int quantity)
{
  Node *node = malloc (sizeof (Node));
  if (!node)
  {
    return NULL;
  }
  node->right_child = NULL;
  node->left_child = NULL;
  char *new_name = malloc (strlen (name) + 1);//free
  if (!new_name)
  {
    delete_tree(node);
    return NULL;
  }
  strcpy (new_name, name);
  Product new_prod;
  new_prod.name = new_name;
  new_prod.quantity = quantity;
  node->product = new_prod;
  return node;
}

Node *add_product_helper (Node *root, char *name, int quantity, Node
*real_root)
{

  int compare = strcmp (name, root->product.name);
  if (!compare)
  {
    //unchanged
    return real_root;
  }
  if (!root->right_child && !root->left_child)
  {
    Node *new_node = node_aloc (name, quantity);
    if (compare > EQUAL && new_node)
    {

      root->right_child = new_node;
    }
    else if (compare < EQUAL && new_node)
    {
      root->left_child = new_node;
    }
  }
  if (compare > EQUAL)
  {
    if (!root->right_child)
    {
      Node *new_node = node_aloc (name, quantity);
      root->right_child = new_node;
      return real_root;
    }
    add_product_helper (root->right_child, name, quantity, real_root);
  }
  else
  {
    if (!root->left_child)
    {
      Node *new_node = node_aloc (name, quantity);
      root->left_child = new_node;
      return real_root;
    }
    add_product_helper (root->left_child, name, quantity, real_root);
  }
}

Node *add_product (Node *root, char *name, int quantity)
{
  //memory staff
  if (quantity <= 0)
  {
    fprintf (stderr, INVALID_QUANTITY);
    return root;
  }
  if (!name || !strlen (name))
  {
    fprintf (stderr, INVALID_NAME);
    return root;
  }
  if (!root)
  {
    return node_aloc (name, quantity);
  }
  else if (!root->product.name)
  {
    char *new_name = malloc (strlen (name) + 1);//free
    if (!new_name)
    {
      return NULL;
    }
    strcpy (new_name, name);

    root->product.name = new_name;
    root->product.quantity = quantity;
  }
  return add_product_helper (root, name, quantity, root);
}

Node *get_input (char *line, Node *root)
{
  char *token = strtok (line, ":");
  if (!token)
  {
    fprintf (stderr, INVALID_INPUT);
    return root;
  }
  //char *new_name = malloc (sizeof (token) + 1);
  char *num = strtok (NULL, ":");
  char *end;
  if (!strcmp (num, "0"))
  {
    fprintf (stderr, INVALID_INPUT);
    return root;
  }
  int int_num = strtol (num, &end, 10);
  if (!int_num)
  {
    fprintf (stderr, INVALID_INPUT);
    return root;
  }
  //new_name = token;
  return add_product (root, token, int_num);
}

Node *build_bst (const char *filename)
{
  if (!filename)
  {
    return NULL;
  }
  FILE *in = fopen (filename, "r");
  if (!in)
  {
    fprintf (stderr, INVALID_FILE);
    return NULL;
  }
  char line[MAX_LINE_LENGTH + 1];
  Node *root = NULL;
  while (fgets (line, MAX_LINE_LENGTH + 1, in))
  {
    root = get_input (line, root);
  }
  fclose (in);
  return root;
}

Node *find_min (Node *root)
{
  if (!root)
  {
    return NULL;
  }
  if (!root->left_child)
  {
    return root;
  }
  return root->left_child;
}
//add possible that there is no father
Node *find_father (Node *root, char *name)
{
  int cmp = strcmp (name, root->product.name);
  if ((!root->right_child && !root->left_child) || !cmp)
  {
    return root;
  }

  if (!root->left_child && cmp > EQUAL)
  {
    return find_father (root->right_child, name);
  }
  else if (!root->right_child && cmp < EQUAL)
  {
    return find_father (root->left_child, name);
  }
    //has 2 kids
  else
  {
    if (!strcmp (name, root->right_child->product.name) ||
        !strcmp (name, root->left_child->product.name))
    {
      return root;
    }
    if (cmp > EQUAL)
    {
      return find_father (root->right_child, name);
    }
    else
    {
      return find_father (root->left_child, name);
    }
  }
}

void swap (Node *p1, Node *p2)
{
  Product temp = p1->product;
  p1->product = p2->product;
  p2->product = temp;
}
//should get a pointer to root of father Node of the one we should erase
void del_one_kid (Node *root, char *name)
{

  if (!strcmp (root->left_child->product.name, name))
  {
    Node *node_to_free = root->left_child;
    if (root->left_child->left_child)
    {

      root->left_child = root->left_child->left_child;
    }
    else if (root->left_child->right_child)
    {
      root->left_child = root->left_child->right_child;
    }
    free_Node (node_to_free);
  }
  else if (!strcmp (root->right_child->product.name, name))
  {
    Node *node_to_free = root->right_child;
    if (root->right_child->right_child){
        root->right_child = root->right_child->right_child;
    }

    else if (root->right_child->left_child)
    {
      root->right_child = root->right_child->left_child;

    }
    free_Node (node_to_free);
  }

}

//should get a pointer to root of father Node of the one we should erase
void del_without_kids (Node *root, char *name)
{
  if (!strcmp (root->product.name, name))
  {
    free_Node(root);
    root->right_child=NULL;
    root->left_child = NULL;
    root = NULL;
  }
  else if (!strcmp (root->left_child->product.name, name))
  {
    free_Node(root->left_child);
    root->left_child = NULL;
  }
  else if (!strcmp (root->right_child->product.name, name))
  {
    free_Node(root->right_child);
    root->right_child = NULL;

  }
}

void free_Node (Node *node)
{
  free (node->product.name);
  node->product.name = NULL;
  free (node);
  node = NULL;
}

void delete_tree (Node *root)
{
  if(root){
    delete_tree (root->left_child);
    root->left_child=NULL;
    delete_tree (root->right_child);
    root->right_child=NULL;
    free (root->product.name);
    root->product.name=NULL;
    free (root);
    root = NULL;
  }

}

Node *delete_product (Node *root, char *name)
{
  return delete_product_helper (root, name, root,0);
}
Node *delete_product_helper (Node *root, char *name, Node *real_root,int
kids)
{

  Node *sub_tree_1 = find_father (root, name);

  if(kids==2){
  sub_tree_1 = find_father (root->right_child, name);
  }

  //has no kids
  int cmp = strcmp (name, sub_tree_1->product.name);
  if ((!sub_tree_1->right_child->right_child) &&
      (!sub_tree_1->right_child->left_child
      ) && cmp > 0)
  {//TODO its in here somwhere!!
    del_without_kids (sub_tree_1, name);
    return real_root;
  }
  else if ((!sub_tree_1->left_child->left_child) && (!sub_tree_1->left_child
      ->right_child) && cmp < EQUAL)
  {
    del_without_kids (sub_tree_1, name);
    return real_root;
  }
    //has one kid
  else if ((!(sub_tree_1->right_child->left_child && sub_tree_1->right_child
      ->right_child)&&cmp>0)||(!(sub_tree_1->left_child->left_child &&
      sub_tree_1->left_child
      ->right_child)&&cmp<0))//TODO-fix
  {
    del_one_kid (sub_tree_1, name);
    return real_root;
  }
    //has 2 kids
  else
  {
    if(!cmp){
      Node *min_sub_tree =
          find_min (sub_tree_1->right_child);
      swap (min_sub_tree, sub_tree_1);
      int kids = 2;
      delete_product_helper (sub_tree_1, min_sub_tree->product.name,
                             real_root,kids);
      return real_root;
    }
    if (sub_tree_1->right_child)
    {
      if (!strcmp (sub_tree_1->right_child->product.name, name))
      {
        Node *min_sub_tree =
            find_min (sub_tree_1->right_child->right_child);
        swap (min_sub_tree, sub_tree_1->right_child);
        int kids = 2;
        delete_product_helper (sub_tree_1, min_sub_tree->product.name,
                               real_root,kids);
        return real_root;
      }
    }
    else
    {
      Node *min_sub_tree =
          find_min (sub_tree_1->left_child->right_child);
      swap (min_sub_tree, sub_tree_1->left_child);
      int kids = 2;
      delete_product_helper (sub_tree_1, min_sub_tree->product.name,
                             real_root,kids);
      return real_root;
      //??
    }
  }
}

//maybe update
Product *search_product (Node *root, char *name)
{
  if (!root || !name)
  {
    return NULL;
  }
  int compare;
  compare = strcmp (name, root->product.name);
  if (!compare)
  {
    return &root->product;
  }
  else if (compare > 0)
  {
    search_product (root->right_child, name);
  }
  else
  {
    search_product (root->left_child, name);
  }
}

Node *update_quantity (Node *root, char *name, int amount_to_update)
{
  Node *p1 = find_father (root, name);
  int cmp = strcmp (name,p1->product.name);
  if(cmp>0){
    if(p1->right_child->product.quantity+amount_to_update>0){
      p1->right_child->product.quantity += amount_to_update;
      return root;
    }
    else if(p1->right_child->product.quantity+amount_to_update==0){
      return delete_product (root,name);
    }
    else if(p1->right_child->product.quantity+amount_to_update<0){
      fprintf(stderr,INVALID_QUANTITY);
      return root;
    }

  }
  else if(!cmp){
    if(p1->product.quantity+amount_to_update>0){
      p1->right_child->product.quantity += amount_to_update;
      return root;
    }
    else if(p1->product.quantity+amount_to_update==0){
      return delete_product (root,name);
    }
    else if(p1->product.quantity+amount_to_update<0){
      fprintf(stderr,INVALID_QUANTITY);
      return root;
    }
  }
  else if(cmp<0){
    if(p1->left_child->product.quantity+amount_to_update>0){
      p1->left_child->product.quantity += amount_to_update;
      return root;
    }
    else if(p1->left_child->product.quantity+amount_to_update==0){
      return delete_product (root,name);
    }
    else if(p1->left_child->product.quantity+amount_to_update<0){
      fprintf(stderr,INVALID_QUANTITY);
      return root;
  }
  }
}