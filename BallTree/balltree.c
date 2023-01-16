#include <stdio.h>
#include <stdlib.h>

struct Node {
  double *data;   // Array of data points in the node
  int size;       // Number of data points in the node
  double radius;  // Radius of the ball enclosing the data points
  struct Node *left;  // Left child node
  struct Node *right; // Right child node
};

struct BallTree {
  struct Node *root;  // Root node of the tree
};

// Function to create a new node
struct Node* newNode(double *data, int size, double radius) {
  struct Node *node = (struct Node*)malloc(sizeof(struct Node));
  node->data = data;
  node->size = size;
  node->radius = radius;
  node->left = NULL;
  node->right = NULL;
  return node;
}

// Function to create a new ball tree
struct BallTree* newTree() {
  struct BallTree *tree = (struct BallTree*)malloc(sizeof(struct BallTree));
  tree->root = NULL;
  return tree;
}

// Function to insert a data point into the tree
void insert(struct BallTree *tree, double *point) {
  // Check if the tree is empty
  if (tree->root == NULL) {
    double *data = (double*)malloc(sizeof(double) * DIM);
    memcpy(data, point, sizeof(double) * DIM);
    tree->root = newNode(data, 1, 0);
    return;
  }

  struct Node *curr = tree->root;
  while (1) {
    // Find the dimension with the largest spread
    int dim = 0;
    double spread = -1;
    for (int i = 0; i < DIM; i++) {
      double curr_spread = fabs(curr->data[i] - curr->data[i + DIM - 1]);
      if (curr_spread > spread) {
        spread = curr_spread;
        dim = i;
      }
    }

    // Find the median value along the chosen dimension
    int median_index = (curr->size - 1) / 2;
    double median_value = curr->data[median_index * DIM + dim];

    // Decide which child to recurse into
    if (point[dim] < median_value) {
      if (curr->left == NULL) {
        double *data = (double*)malloc(sizeof(double) * DIM);
        memcpy(data, point, sizeof(double) * DIM);
        curr->left = newNode(data, 1, 0);
        break;
      }
      curr = curr->left;
    } else {
      if (curr->right == NULL) {
        double *data = (double*)malloc(sizeof(double) * DIM);
        memcpy(data, point, sizeof(double) * DIM);
        curr->right = newNode(data, 1, 0);
        break;
      }
      curr = curr->right;
    }
  }
  // Update the radius of the current node
  curr->radius = max_distance(point, curr->data, curr->size);
}


// Function to search for nearest neighbors in the tree
void search(struct BallTree *tree, double *query, int k, double *results) {
  // Initialize a priority queue to store the potential nearest neighbors
  struct PriorityQueue *queue = newPriorityQueue(k);

  // Start the search at the root of the tree
  searchNode(tree->root, query, k, queue);

  // While the priority queue is not empty, continue to check the child nodes
  while (!isEmpty(queue)) {
    struct Node *node = (struct Node*)pop(queue);

    // If the current node is a leaf, add its data points to the results
    if (node->left == NULL && node->right == NULL) {
      for (int i = 0; i < node->size; i++) {
        double *point = node->data + i * DIM;
        double distance = periodic_euclidean_distance(query, point, DIM);
        insert_neighbor(results, k, point, distance);
      }
      continue;
    }

    // Check the left child node
    if (node->left != NULL) {
      double distance = periodic_euclidean_distance(query, node->left->data, DIM);
      if (distance <= get_distance(results, k)) {
        searchNode(node->left, query, k, queue);
      }
    }

    // Check the right child node
    if (node->right != NULL) {
      double distance = periodic_euclidean_distance(query, node->right->data, DIM);
      if (distance <= get_distance(results, k)) {
        searchNode(node->right, query, k, queue);
      }
    }
  }

  // Cleanup
  deletePriorityQueue(queue);
}

// Function to calaculate the periodic euclidean distance
double periodic_euclidean_distance(double *point1, double *point2, double *box_size, int DIM) {
  double distance_sq = 0;
  for (int i = 0; i < DIM; i++) {
    double delta = fabs(point1[i] - point2[i]);
    double box = box_size[i];
    if (delta > box / 2) {
      delta = box - delta;
    }
    distance_sq += delta * delta;
  }
  return sqrt(distance_sq);
}

// Function to search nodes in a Ball tree
void searchNode(struct Node *node, double *query, int k, struct PriorityQueue *queue) {
  double distance = periodic_euclidean_distance(node->data, query, box_size, DIM);
  if (distance <= node->radius) {
    double farthest_distance = get_distance(queue);
    if (distance < farthest_distance) {
      insert(queue, node, distance);
    }
  }
}

// Function to get distance
double get_distance(struct PriorityQueue *queue) {
  if (queue->size > 0) {
    return queue->elements[queue->size-1].distance;
  }
  return -1;
}




int main() {
  // Example usage of the ball tree
  struct BallTree *tree = newTree();
  double data[] = {1, 2, 3, 4, 5};
  insert(tree, data);
  double query[] = {2, 3};
  double results[5];
  search(tree, query, 5, results);
  return 0;
}
