#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "3DViewer.h"

matrix_t fill_matrix_t_from_arr(const double src[][3], size_t rows) {
  matrix_t matrix;
  matrix.rows = rows;
  matrix.column = 3;
  matrix.max = 0;
  matrix.matrix = (double **)malloc(rows * sizeof(double *));
  for (size_t i = 0; i < rows; i++) {
    matrix.matrix[i] = (double *)malloc(3 * sizeof(double));
  }

  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < 3; j++) {
      matrix.matrix[i][j] = src[i][j];
    }
  }

  return matrix;
}

polygon_t FillPolygonFromArr(const int src[], size_t size) {
  polygon_t polygon;
  polygon.size = size * 2;

  polygon.polygon = (int *)malloc(size * sizeof(int));
  for (size_t i = 0; i < size; i++) {
    polygon.polygon[i] = src[i];
  }

  return polygon;
}

void objCompare(object_t my_obj, object_t not_my_obj) {
  for (size_t i = 0; i < my_obj.matrix3d.rows; i++) {
    for (size_t j = 0; j < my_obj.matrix3d.column; j++) {
      ck_assert_double_eq_tol(my_obj.matrix3d.matrix[i][j],
                              not_my_obj.matrix3d.matrix[i][j], 1e-2);
    }
  }
}

object_t CubeInit() {
  double matrix_arr[][3] = {
      {1.000000, -1.000000, -1.000000}, {1.000000, -1.000000, 1.000000},
      {-1.000000, -1.000000, 1.000000}, {-1.000000, -1.000000, -1.000000},
      {1.000000, 1.000000, -0.999999},  {0.999999, 1.000000, 1.000001},
      {-1.000000, 1.000000, 1.000000},  {-1.000000, 1.000000, -1.000000},
  };

  matrix_t matrix = fill_matrix_t_from_arr(
      matrix_arr, sizeof(matrix_arr) / sizeof(matrix_arr[0]));

  int polygon_arr[] = {2, 3, 4, 8, 7, 6, 5, 6, 2, 6, 7, 3, 3, 7, 8, 1, 4, 8,
                       1, 2, 4, 5, 8, 6, 1, 5, 2, 2, 6, 3, 4, 3, 8, 5, 1, 8};
  int polygon_size = sizeof(polygon_arr) / sizeof(polygon_arr[0]);

  polygon_t polygon = FillPolygonFromArr(polygon_arr, polygon_size);
  object_t result = {matrix.rows, 0, matrix, polygon};
  return result;
}

START_TEST(parsing_test) {
  object_t my_obj = parsing("objfiles/cube.obj");
  object_t not_my_obj = CubeInit();

  for (size_t i = 0; i < my_obj.matrix3d.rows; i++) {
    for (size_t j = 0; j < my_obj.matrix3d.column; j++) {
      ck_assert_double_eq_tol(my_obj.matrix3d.matrix[i][j],
                              not_my_obj.matrix3d.matrix[i][j], 1e-6);
    }
  }

  objCompare(my_obj, not_my_obj);
  remove_object(&my_obj);
  remove_object(&not_my_obj);
}
END_TEST

START_TEST(zoom_matrix_test) {
  object_t my_obj = parsing("objfiles/cube.obj");
  object_t not_my_obj = CubeInit();
  int num = 3;

  for (size_t i = 0; i < not_my_obj.matrix3d.rows; i++) {
    for (size_t j = 0; j < not_my_obj.matrix3d.column; j++) {
      not_my_obj.matrix3d.matrix[i][j] = not_my_obj.matrix3d.matrix[i][j] * num;
    }
  }

  zoom_matrix(&my_obj.matrix3d, num);
  objCompare(my_obj, not_my_obj);

  remove_object(&my_obj);
  remove_object(&not_my_obj);
}
END_TEST

START_TEST(move_matrix_test) {
  object_t my_obj = parsing("objfiles/cube.obj");
  object_t not_my_obj = CubeInit();
  int num = 3;
  int axis = 2;

  for (size_t i = 0; i < not_my_obj.matrix3d.rows; i++) {
    not_my_obj.matrix3d.matrix[i][axis] += num;
  }

  move_matrix(&my_obj.matrix3d, num, axis);
  objCompare(my_obj, not_my_obj);

  remove_object(&my_obj);
  remove_object(&not_my_obj);
}
END_TEST

START_TEST(rotation_test) {
  object_t my_obj = parsing("objfiles/cube.obj");
  object_t not_my_obj = CubeInit();

  rotation_matrix(&my_obj.matrix3d, 360 / 57.2958, X);
  rotation_matrix(&my_obj.matrix3d, 360 / 57.2958, Y);
  rotation_matrix(&my_obj.matrix3d, 360 / 57.2958, Z);
  objCompare(my_obj, not_my_obj);

  remove_object(&my_obj);
  remove_object(&not_my_obj);
}
END_TEST

Suite *sfleta_suite(void) {
  Suite *suite = suite_create("3DViewer");
  TCase *tcase_core = tcase_create("Tests");
  tcase_add_test(tcase_core, parsing_test);
  tcase_add_test(tcase_core, zoom_matrix_test);
  tcase_add_test(tcase_core, move_matrix_test);
  tcase_add_test(tcase_core, rotation_test);

  suite_add_tcase(suite, tcase_core);
  return suite;
}

int main(void) {
  Suite *suite = sfleta_suite();
  SRunner *suite_runner = srunner_create(suite);
  srunner_run_all(suite_runner, CK_NORMAL);
  int failed_count = srunner_ntests_failed(suite_runner);
  srunner_free(suite_runner);
  return (failed_count != 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}
