//
// Created by naqni on 06/11/2022.
//
#include "productBST.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define MAX_SIZE_LINE 1024
int name_aloc (char *name, char *dest);
Node *get_input (char *line, Node *root);
Node *node_aloc (char *name, int quantity);
void swap (Node *p1, Node *p2);
Node *delete_product_helper (Node *root, char *name, Node *real_root);
Node *add_product_helper (Node *root, char *name, int quantity, Node
*real_root);
void del_without_kids (Node *root, char *name);
Node *find_min (Node *root);
void free_Node (Node *node);

//int name_aloc (char *name, char *dest)
//{
//  dest = malloc (sizeof (strlen (name)) + 1);
//  if (!dest)
//  {
//    return 1;
//  }
//  strcpy (dest, name);
//  return 0;
//}

Node *node_aloc (char *name, int quantity)
{
  Node *node = malloc (sizeof (Node));
  if (!node)
  {
    return NULL;
  }
  char* new_name = malloc (sizeof (strlen (name)) + 1);
  if (!new_name)
  {
    return 1;
  }
  strcpy (new_name, name);
  Product new_prod;
  new_prod.name = new_name;
//  if (name_aloc (name, node->product.name))
//  {
//    free_Node (node);
//    return NULL;
//  }
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
    Node *new_node = node_aloc (name,quantity);
    if (compare > 0 && new_node)
    {
      root->right_child = new_node;
    }
    else if (compare < 0 && new_node)
    {
      root->left_child = new_node;
    }
  }
  if (compare > 0)
  {
    add_product_helper (root->right_child, name, quantity, real_root);
  }
  else
  {
    add_product_helper (root->left_child, name, quantity, real_root);
  }
}

Node *add_product (Node *root, char *name, int quantity)
{
  //memory staff
  if (quantity <= 0)
  {
    fprintf (stderr, "the quantity is zero or negative, please try again");
    return root;
  }
  if (!name || !strlen (name))
  {
    fprintf (stderr, "invalid name");
    return root;
  }
  if (!root)
  {
    return node_aloc (name,quantity);
//    root = node;
//    root->product.quantity = quantity;
//    return root;
  }

  return add_product_helper (root, name, quantity, root);

}

Node *get_input (char *line, Node *root)
{
  char *token = strtok (line, ":");
  if (!token)
  {
    fprintf (stderr, "invalid input");
    return root;
  }
  char *new_name = malloc (sizeof (token));
  char *num = strtok (NULL, ":");
  char *end;
  if (!strcmp (num, "0"))
  {
    fprintf (stderr, "invalid input");
    return root;
  }
  int int_num = strtol (num, &end, 10);
  if (!int_num)
  {
    fprintf (stderr, "invalid input");
    return root;
  }
  new_name = token;
  return add_product (root, new_name, int_num);
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
    fprintf (stderr, "The given file is invalid.\n");
  }
  char line[MAX_LINE_LENGTH];
  Node *root = NULL;
  while (fgets (line, MAX_LINE_LENGTH + 1, in))
  {
    root = get_input (line, root);
  }
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
  if (root->left_child && root->right_child)
  {
    if (!strcmp (root->left_child->product.name, name) || (
        !strcmp (root->right_child->product.name, name)))
    {
      return root;
    }
    else if (strcmp (root->product.name, name) > 0)
    {
      find_father (root->right_child, name);
    }
    else if (strcmp (root->product.name, name) < 0)
    {
      find_father (root->left_child, name);
    }
  }
  return root;
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
    if (root->left_child->left_child)
    {
      root->left_child = root->left_child->left_child;
    }
    else if (root->left_child->left_child)
    {
      root->left_child = root->left_child->right_child;
    }
  }
  else if (!strcmp (root->right_child->product.name, name))
  {
    if (root->right_child->left_child)
    {
      root->right_child = root->right_child->left_child;
    }
    else if (root->right_child->left_child)
    {
      root->right_child = root->right_child->right_child;
    }

  }
}

//should get a pointer to root of father Node of the one we should erase
void del_without_kids (Node *root, char *name)
{
  if (!strcmp (root->left_child->product.name, name))
  {
    root->left_child = NULL;
  }
  else if (!strcmp (root->right_child->product.name, name))
  {
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
  if (root)
  {
    if (root->left_child)
    {
      delete_tree (root->left_child);
    }
    if (root->right_child)
    {
      delete_tree (root->right_child);
    }
    free_Node (root);
  }
}

Node *delete_product (Node *root, char *name)
{
  return delete_product_helper (root, name, root);
}
Node *delete_product_helper (Node *root, char *name, Node *real_root)
{
  Node *sub_tree_1 = find_father (root, name);
  //has no kids
  if ((!sub_tree_1->right_child->left_child) && (!sub_tree_1->right_child
      ->right_child))
  {
    del_without_kids (sub_tree_1, name);
    return real_root;
  }
    //has one kid
  else if (!(sub_tree_1->right_child->left_child && sub_tree_1->right_child
      ->right_child))
  {
    del_one_kid (sub_tree_1, name);
    return real_root;
  }
    //has 2 kids
  else
  {
    if (sub_tree_1->right_child)
    {
      if (!strcmp (sub_tree_1->right_child->product.name, name))
      {
        Node *min_sub_tree =
            find_min (sub_tree_1->right_child->right_child);
        swap (min_sub_tree, sub_tree_1->right_child);
        delete_product_helper (sub_tree_1, min_sub_tree->product.name,
                               real_root);
      }
    }
    else
    {
      Node *min_sub_tree =
          find_min (sub_tree_1->left_child->right_child);
      swap (min_sub_tree, sub_tree_1->left_child);
      delete_product_helper (sub_tree_1, min_sub_tree->product.name,
                             real_root);
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
  if (root == p1)
  {
    root->product.quantity += amount_to_update;
    return root;
  }
  if (!strcmp (p1->left_child->product.name, name))
  {
    p1->left_child->product.quantity += amount_to_update;
  }
  else
  {
    p1->right_child->product.quantity += amount_to_update;
  }
}
