#include "mpi.h"
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <time.h>
#include <math.h>

#define POINTS 1000000

int main (int argc,  char *argv[]) {

   int myid, numprocs;
   int namelen;


   char processor_name[MPI_MAX_PROCESSOR_NAME];

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
   MPI_Comm_rank(MPI_COMM_WORLD, &myid);
   MPI_Get_processor_name(processor_name, &namelen);
 


   printf("Process %d on %s\n", myid, processor_name);
 

   int s = (int)floor(POINTS/numprocs);
   int s0 = s + POINTS%numprocs;

   int startIndex = s0 + (myid-1)*s;
   int endIndex = startIndex + s;

   double startwtime;

   srand((unsigned)(myid+1)); // Create a random seed

   if (myid == 0) 
   {
      startwtime = MPI_Wtime();
   }


   int pointsInCircle = 0; /* This will hold the number of dots that fall
                              in our circle */

   
   if (myid == 0) 
   {

      // master worker - comput the master's numbers

      srand((unsigned)(myid+1)); // Create a random seed

      for (int i=0; i<s0; i++) 
      {

         float xCoordinate = ( (double) rand() ) / ( (double)RAND_MAX);
         float YCoordinate = ( (double) rand() ) / ( (double)RAND_MAX);

         if ( (pow(xCoordinate, 2.0) + pow(YCoordinate, 2.0)) <= 1.0)
         {
            pointsInCircle++;
         }
         
      }
      printf("\nProcess %d - startIndex 0 endIndex %d\nPoints Generated = %d : Points in Circle = %d\n",
             myid, s0-1, s0, pointsInCircle);


   } else 
   {
      //slave's work

      srand((unsigned)(myid+1)); // Create a random seed
      

      for (int i= startIndex; i<endIndex; i++) 
      {
         
         float xCoordinate = ( (double) rand() ) / ( (double)RAND_MAX);
         float YCoordinate = ( (double) rand() ) / ( (double)RAND_MAX);

         if ( (pow(xCoordinate, 2.0) + pow(YCoordinate, 2.0)) <= 1.0)
         {
            pointsInCircle++;
         }

      }

      
      printf("\nProcess %d - startIndex 0 endIndex %d\nPoints Generated = %d : Points in Circle = %d\n",
             myid, s0-1, s0, pointsInCircle);
   }

   int TotalCirclePoints = 0.0;
   MPI_Reduce(&pointsInCircle, &TotalCirclePoints, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

   float fPoints = 1.0 * POINTS;          // Convert the int to a float for calculation purposes
   float fTCP = 1.0 * TotalCirclePoints;  // Convert the int to a float for calcualtion purposes

   if (myid == 0) {
      double runTime = MPI_Wtime() - startwtime;
      printf("\nExecution time (sec) = %f \n\nTotal Points = %ld Points In Circle Area = %ld\n",
             runTime, POINTS, TotalCirclePoints);

      float ratio = fTCP/fPoints;
      float ourPi = 4.0 * ratio;

      printf("\nRatio = %f \nOur Estimated Pi = %f\n",
               ratio, ourPi);

      float piDelta = M_PI - ourPi;
      printf("\nPi Delta (Difference from true Pi) = %f\n", piDelta);
   }


   MPI_Finalize();
}

