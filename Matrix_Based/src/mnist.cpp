#include "../include/nn.h"
#include "../include/mnist_parser.h"
#include <cstddef>
#include <typeinfo>

#define OUTPUT_SIZE 10
#define IMAGE_SIZE 784

// :TODO add a slice method to the matrix class
// implement: Xavier/Glorot initialization
// Pixel normaliziation within parser?
// Look at batch extraction methods which overwrites pointer to the internal array (Might be a very dirty solution)
//

int main() 
{
  // config
  constexpr size_t number_of_layers = 4;
  constexpr size_t layer_config[number_of_layers] = {IMAGE_SIZE, 16, 16, OUTPUT_SIZE};
  const size_t batch_size = 32;
  constexpr bool use_one_hot = true;
  double lr = 0.0001;
  double epochs = 10;
  bool verbose = true;


  // Initialize Neural Network
  nn mlp(number_of_layers, layer_config, batch_size, use_one_hot);

  // Read the MNIST dataset
  // Train data
  Matrix* mnist_data = read_mnist("train"); 
  size_t n_images_train = mnist_data->n_rows;

  // Divide into label and input data
  Matrix* true_lables = mnist_data->select_col(IMAGE_SIZE);
  Matrix* x_data = mnist_data->slice(0, n_images_train - 1, 0, IMAGE_SIZE - 1); // select the first 784 cols
  // Test data
  Matrix* mnist_data_test = read_mnist("test");
  size_t n_images_test = mnist_data_test->n_rows;

  // Divide into label and input data
  Matrix* true_lables_test = mnist_data_test->select_col(IMAGE_SIZE);
  Matrix* x_data_test = mnist_data_test->slice(0, n_images_test - 1, 0, IMAGE_SIZE - 1); // select the first 784 cols
  
  // Clean up
  delete mnist_data;
  delete mnist_data_test;

  // ***** Train the Model ***** //
  mlp.train(x_data, true_lables, lr, epochs, verbose, x_data_test, true_lables_test);
  

  // ***** Calculate test accuracy for the test data ***** //
  Matrix* full_pred = mlp.forward(x_data_test)->softmax();
  size_t output_size = full_pred->n_cols;
  size_t correct_predictions = 0;
  double test_accuracy;
  size_t number_of_test_images = x_data_test->n_rows;
  size_t number_of_categories = full_pred->n_cols;
  std::vector<size_t> predicted_index(number_of_test_images);
  
  for (size_t row = 0; row < number_of_test_images; row++) {
    double highest_prob = 0;
    // Inner Loop
    for (size_t col = 0 ; col < number_of_categories; col++) {
      if (full_pred->at(row, col) > highest_prob) {
        highest_prob = full_pred->at(row, col);
        predicted_index[row] = col;
      }
    }
    // Outer Loop 
    if (predicted_index[row] == static_cast<size_t>(true_lables_test->at(row, 0))){
      correct_predictions++;
    }
  }
  test_accuracy = static_cast<double>(correct_predictions) / number_of_test_images;
  
  std::cout << "Test Accuracy: " << test_accuracy << std::endl;

  
  Matrix* f = mlp.forward(x_data_test->slice(50, 60, 0, IMAGE_SIZE - 1));
  for (size_t i = 0; i < 11; i++) {
    double highest_prob = 0;
    for (size_t j = 0; j < output_size; j++) {
      if (f->at(i, j) > highest_prob) {
        highest_prob = full_pred->at(i, j);
        predicted_index[i] = j;
      }
    }
    std::cout << "Pred: " << predicted_index[i] << " vs. " << true_lables_test->slice(50, 60, 0, 0)->at(i, 0) << std::endl;
  }
  return 0;
}
