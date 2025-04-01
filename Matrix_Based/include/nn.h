#ifndef NN_H
#define NN_H

#include "value_matrix.h"
#include <cblas.h> // OpenBLAS header
#include <cassert>
#include <cmath>
#include <openblas_config.h>
#include <omp.h>


class nn {
private:
  // Array of Matrices where the indice correspond to the layer
  Matrix** layer_weights;
  Matrix** layer_biases;
  size_t num_layers;
  size_t context_size;
  size_t output_size;
  size_t batch_size;
  bool use_one_hot;

public:
  nn(const size_t number_of_layers, const size_t layer_sizes[], const size_t batch_size, const bool use_one_hot);
  ~nn();

  Matrix* mse_loss(Matrix* y_pred, Matrix* y_true);
  Matrix* cce_loss(Matrix* y_pred, Matrix* y_true);
  void update(double& lr);
  void train(Matrix* x, Matrix* y, double lr, double epochs, bool verbose, Matrix* test_data);
  void print() const;

  Matrix* forward(Matrix* input);
  void predict(Matrix* input);

  
  Matrix* one_hot(Matrix* x);
};

#endif // !NN_H
