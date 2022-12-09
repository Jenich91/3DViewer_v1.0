#include "3DViewer.h"

#include <ctype.h>

object_t parsing(char *objfile) {
  size_t rows_mat = 0, len = 0, size_pol = 0, count_of_facets = 0;
  double max = EPS;
  char *line = NULL;
  FILE *File = fopen(objfile, "r");
  int *polygon = (int *)calloc(1, sizeof(int));
  double **matrix = (double **)calloc(1, sizeof(double *));

  while (File && getline(&line, &len, File) != -1) {
    if (*line == 'v' && *(line + 1) == ' ') {
      create_matrix(line + 1, &matrix, rows_mat++, &max);
    } else if (*line == 'f') {
      create_polygon(line + 1, &polygon, &size_pol);
      count_of_facets++;
    }
  }
  free(line);
  fclose(File);

  object_t result = {rows_mat,
                     count_of_facets,
                     {matrix, max, rows_mat, 3},
                     {polygon, size_pol}};
  return result;
}

void create_matrix(char *line, double ***value, size_t row, double *max) {
  *value = (double **)realloc(*value, (row + 1) * sizeof(double *));
  (*value)[row] = (double *)calloc(3, sizeof(double));
  for (int i = 0; *(line++) != '\n'; i++) {
    (*value)[row][i] = strtod(line, &line);
    if ((*value)[row][i] > *max) *max = (*value)[row][i];
  }
}

void create_polygon(char *line, int **value, size_t *size) {
  size_t i = 0, pos0 = *size;
  for (; *(line) && *(line++) != '\n';) {
    if (*(line - 1) == '/') {
      (void)strtod(line, &line);
    } else if (isdigit(*line)) {
      *value = (int *)realloc(*value, sizeof(int) * (*size + 1));
      (*value)[(*size)++] = strtod(line, &line);
      if (i++ > 0) {
        *value = (int *)realloc(*value, sizeof(int) * (*size + 1));
        (*value)[(*size)] = (*value)[*size - 1];
        (*size)++;
      }
    }
  }
  *value = (int *)realloc(*value, sizeof(int) * (*size + 1));
  (*value)[(*size)++] = (*value)[pos0];
}

void zoom_matrix(matrix_t *value, double number) {
  if (number > EPS) {
    for (size_t i = 0; i < value->rows; i++) {
      for (size_t j = 0; j < value->column; j++) {
        value->matrix[i][j] *= number;
      }
    }
  }
}

void move_matrix(matrix_t *value, double number, int axis) {
  if (axis > -1 && axis < 3) {
    for (size_t i = 0; i < value->rows; i++) {
      value->matrix[i][axis] += number;
    }
  }
}

void rotation_matrix(matrix_t *obj, double angle, int asix) {
  if (asix == X) {
    for (size_t i = 0; i < obj->rows; i++) {
      double tmp_y = obj->matrix[i][Y];
      double tmp_z = obj->matrix[i][Z];
      obj->matrix[i][Y] = cos(angle) * tmp_y - sin(angle) * tmp_z;
      obj->matrix[i][Z] = sin(angle) * tmp_y + cos(angle) * tmp_z;
    }
  } else if (asix == Y) {
    for (size_t i = 0; i < obj->rows; i++) {
      double tmp_x = obj->matrix[i][X];
      double tmp_z = obj->matrix[i][Z];
      obj->matrix[i][X] = cos(angle) * tmp_x + sin(angle) * tmp_z;
      obj->matrix[i][Z] = -sin(angle) * tmp_x + cos(angle) * tmp_z;
    }
  } else if (asix == Z) {
    for (size_t i = 0; i < obj->rows; i++) {
      double tmp_x = obj->matrix[i][X];
      double tmp_y = obj->matrix[i][Y];
      obj->matrix[i][X] = cos(angle) * tmp_x - sin(angle) * tmp_y;
      obj->matrix[i][Y] = sin(angle) * tmp_x + cos(angle) * tmp_y;
    }
  }
}

void remove_matrix(matrix_t *value) {
  for (size_t i = 0; i < value->rows; i++) {
    free(value->matrix[i]);
  }
  free(value->matrix);
}

void remove_polygon(polygon_t *value) { free(value->polygon); }

void remove_object(object_t *value) {
  remove_matrix(&value->matrix3d);
  remove_polygon(&value->polygon);
}
