/* Kalman filters. */

#include "algorithm/inc/algo_kalman.h"

KalmanFilter alloc_filter(int state_dimension, int observation_dimension)
{
    KalmanFilter f;
    f.timestep              = 0;
    f.state_dimension       = state_dimension;
    f.observation_dimension = observation_dimension;

    f.state_transition             = alloc_matrix(state_dimension, state_dimension);
    f.observation_model            = alloc_matrix(observation_dimension, state_dimension);
    f.process_noise_covariance     = alloc_matrix(state_dimension, state_dimension);
    f.observation_noise_covariance = alloc_matrix(observation_dimension, observation_dimension);

    f.observation = alloc_matrix(observation_dimension, 1);

    f.predicted_state               = alloc_matrix(state_dimension, 1);
    f.predicted_estimate_covariance = alloc_matrix(state_dimension, state_dimension);
    f.innovation                    = alloc_matrix(observation_dimension, 1);
    f.innovation_covariance         = alloc_matrix(observation_dimension, observation_dimension);
    f.inverse_innovation_covariance = alloc_matrix(observation_dimension, observation_dimension);
    f.optimal_gain                  = alloc_matrix(state_dimension, observation_dimension);
    f.state_estimate                = alloc_matrix(state_dimension, 1);
    f.estimate_covariance           = alloc_matrix(state_dimension, state_dimension);

    f.vertical_scratch     = alloc_matrix(state_dimension, observation_dimension);
    f.small_square_scratch = alloc_matrix(observation_dimension, observation_dimension);
    f.big_square_scratch   = alloc_matrix(state_dimension, state_dimension);

    return f;
}

void free_filter(KalmanFilter f)
{
    free_matrix(f.state_transition);
    free_matrix(f.observation_model);
    free_matrix(f.process_noise_covariance);
    free_matrix(f.observation_noise_covariance);

    free_matrix(f.observation);

    free_matrix(f.predicted_state);
    free_matrix(f.predicted_estimate_covariance);
    free_matrix(f.innovation);
    free_matrix(f.innovation_covariance);
    free_matrix(f.inverse_innovation_covariance);
    free_matrix(f.optimal_gain);
    free_matrix(f.state_estimate);
    free_matrix(f.estimate_covariance);

    free_matrix(f.vertical_scratch);
    free_matrix(f.small_square_scratch);
    free_matrix(f.big_square_scratch);
}

void update(KalmanFilter f)
{
    predict(f);
    estimate(f);
}

void predict(KalmanFilter f)
{
    f.timestep++;

    /* Predict the state */
    multiply_matrix(f.state_transition, f.state_estimate, f.predicted_state);

    /* Predict the state estimate covariance */
    multiply_matrix(f.state_transition, f.estimate_covariance, f.big_square_scratch);
    multiply_by_transpose_matrix(f.big_square_scratch, f.state_transition,
                                 f.predicted_estimate_covariance);
    add_matrix(f.predicted_estimate_covariance, f.process_noise_covariance,
               f.predicted_estimate_covariance);
}

void estimate(KalmanFilter f)
{
    /* Calculate innovation */
    multiply_matrix(f.observation_model, f.predicted_state, f.innovation);
    subtract_matrix(f.observation, f.innovation, f.innovation);

    /* Calculate innovation covariance */
    multiply_by_transpose_matrix(f.predicted_estimate_covariance, f.observation_model,
                                 f.vertical_scratch);
    multiply_matrix(f.observation_model, f.vertical_scratch, f.innovation_covariance);
    add_matrix(f.innovation_covariance, f.observation_noise_covariance, f.innovation_covariance);

    /* Invert the innovation covariance.
       Note: this destroys the innovation covariance.
       TODO: handle inversion failure intelligently. */
    destructive_invert_matrix(f.innovation_covariance, f.inverse_innovation_covariance);

    /* Calculate the optimal Kalman gain.
       Note we still have a useful partial product in vertical scratch
       from the innovation covariance. */
    multiply_matrix(f.vertical_scratch, f.inverse_innovation_covariance, f.optimal_gain);

    /* Estimate the state */
    multiply_matrix(f.optimal_gain, f.innovation, f.state_estimate);
    add_matrix(f.state_estimate, f.predicted_state, f.state_estimate);

    /* Estimate the state covariance */
    multiply_matrix(f.optimal_gain, f.observation_model, f.big_square_scratch);
    subtract_from_identity_matrix(f.big_square_scratch);
    multiply_matrix(f.big_square_scratch, f.predicted_estimate_covariance, f.estimate_covariance);
}
