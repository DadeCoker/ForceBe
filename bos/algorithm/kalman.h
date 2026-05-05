#ifndef __KALMAN_H__
#define __KALMAN_H__

#include "matrix.h"

/* Refer to http://en.wikipedia.org/wiki/Kalman_filter for
   mathematical details. The naming scheme is that variables get names
   that make sense, and are commented with their analog in
   the Wikipedia mathematical notation.
   This Kalman filter implementation does not support controlled
   input.
   (Like knowing which way the steering wheel in a car is turned and
   using that to inform the model.)
   Vectors are handled as n-by-1 matrices.
   TODO: comment on the dimension of the matrices */
typedef struct {
  /* k */
  int timestep;

  /* These parameters define the size of the matrices. */
  int state_dimension, observation_dimension;
  
  /* This group of matrices must be specified by the user. */
  /* F_k */
  Matrix state_transition;
  /* H_k */
  Matrix observation_model;
  /* Q_k */
  Matrix process_noise_covariance;
  /* R_k */
  Matrix observation_noise_covariance;

  /* The observation is modified by the user before every time step. */
  /* z_k */
  Matrix observation;
  
  /* This group of matrices are updated every time step by the filter. */
  /* x-hat_k|k-1 */
  Matrix predicted_state;
  /* P_k|k-1 */
  Matrix predicted_estimate_covariance;
  /* y-tilde_k */
  Matrix innovation;
  /* S_k */
  Matrix innovation_covariance;
  /* S_k^-1 */
  Matrix inverse_innovation_covariance;
  /* K_k */
  Matrix optimal_gain;
  /* x-hat_k|k */
  Matrix state_estimate;
  /* P_k|k */
  Matrix estimate_covariance;

  /* This group is used for meaningless intermediate calculations */
  Matrix vertical_scratch;
  Matrix small_square_scratch;
  Matrix big_square_scratch;
  
} KalmanFilter;

KalmanFilter alloc_filter(int state_dimension,
			  int observation_dimension);

void free_filter(KalmanFilter f);

/* Runs one timestep of prediction + estimation.

   Before each time step of running this, set f.observation to be the
   next time step's observation.

   Before the first step, define the model by setting:
   f.state_transition
   f.observation_model
   f.process_noise_covariance
   f.observation_noise_covariance

   It is also advisable to initialize with reasonable guesses for
   f.state_estimate
   f.estimate_covariance
*/
void update(KalmanFilter f);

/* Just the prediction phase of update. */
void predict(KalmanFilter f);
/* Just the estimation phase of update. */
void estimate(KalmanFilter f);

#endif
