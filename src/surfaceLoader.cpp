#include <iostream>
#include <bits/stdc++.h>
#include <glm/vec3.hpp>

template <typename T>
std::vector<T> linspace(T a, T b, size_t N)
{
   T h = (b - a) / static_cast<T>(N - 1);
   std::vector<T> xs(N);
   typename std::vector<T>::iterator x;
   T val;
   for (x = xs.begin(), val = a; x != xs.end(); ++x, val += h)
      *x = val;
   return xs;
}

int binomialCoeff(int n, int k)
{
   // Base Cases
   if (k == 0 || k == n)
      return 1;

   // Recur
   return binomialCoeff(n - 1, k - 1) +
          binomialCoeff(n - 1, k);
}

double Bernstein(int i, int n, double u)
{

   double *temp = new double[n + 1];

   for (int j = 0; j <= n; ++j)
   {

      temp[j] = 0.0;
   }

   temp[n - i] = 1.0;
   double u1 = 1.0 - u;

   for (int k = 1; k <= n; ++k)
   {

      for (int j = n; j >= k; --j)
      {

         temp[j] = u1 * temp[j] + u * temp[j - 1];
      }
   }

   // std::cout << "B" << i << n << "(" << u << ")"
   //           << ":" << temp[n];
   return temp[n];
}
void generateTriangles(int argc, char const *argv[])
{
   int divisions = 51;
   double umin = 0.0, umax = 1.0, vmin = 0.0, vmax = 1.0;

   int N = 4, M = 4;

   int control_points[16][3] = {
       {0, 0, 80},
       {0, 50, 65},
       {0, 100, 90},
       {0, 150, 85},
       {50, 0, 80},
       {50, 50, 85},
       {50, 100, 90},
       {50, 150, 95},
       {100, 0, 100},
       {100, 50, 105},
       {100, 100, 95},
       {100, 150, 100},
       {150, 0, 90},
       {150, 50, 100},
       {150, 100, 100},
       {150, 150, 85},
   };

   std::vector<double> u1 = linspace(umin, umax, divisions);
   std::vector<double> v1 = linspace(vmin, vmax, divisions);

   glm::vec3 finalControlPointList[N][M];

   int temp = 0;

   for (size_t i = 0; i < N; i++)
   {
      for (size_t j = 0; j < M; j++)
      {
         

         finalControlPointList[i][j] = glm::vec3(control_points[temp][0], control_points[temp][1], control_points[temp][2]);
         temp++;
      }
   }
   // int index1 = 1;
   // int index2 = 1;
   // std::cout << finalControlPointList[index1][index2].x << " " << finalControlPointList[index1][index2].y << " " << finalControlPointList[index1][index2].z << std::endl;

   double u = 0, v = 0, b1, b2;
   glm::vec3 triangle_array[divisions][divisions];
   glm::vec3 temp_vector;

   for (size_t i1 = 0; i1 < divisions; i1++)
   {
      u = u1[i1];
      for (size_t j1 = 0; j1 < divisions; j1++)
      {
         v = v1[j1];
         temp_vector = glm::vec3(0.f);
         for (size_t i = 0; i < N; i++)
         {
            for (size_t j = 0; j < M; j++)
            {
               // b1 = Bernstein(i,N, u);
               // b2 = Bernstein(j, M, v);
               // std::cout << std::fixed<< b1*b2 << std::endl;

               b1 = binomialCoeff(N - 1, i) * pow(u, i) * pow((1 - u), (N - 1 - i)) * binomialCoeff(M - 1, j) * pow(v, j) * pow((1 - v), (M - 1 - j));
               temp_vector += (float)b1*finalControlPointList[i][j];
            }
         }

         triangle_array[i1][j1] = temp_vector;


      }
   }


   int index1 = 0;
   int index2 = 0;
   std::cout << triangle_array[index1][index2].x << " " << triangle_array[index1][index2].y << " " << triangle_array[index1][index2].z << std::endl;

   
}
