SET(LIBALGO_SRC algo_base64.c algo_gps.c algo_hmac_sha1.c algo_kalman.c algo_matrix.c algo_sort
        .c algo_speedpid.c)

INCLUDE_DIRECTORIES(inc../../ Examples / Test / bos_config /../ modules / inc /../ core / inc /)

ADD_LIBRARY(algorithm SHARED ${LIBALGO_SRC})
