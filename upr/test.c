
#include <stdio.h>
 
int main()
{
   int position, i, nbr;
   int arr[100];
 
   printf("Enter the number of elements in the array: ");
   scanf("%d", &nbr);
 
   printf("Enter the %d elements: ", nbr);
 
   for (i = 0; i < nbr; i++)
      scanf("%d", &arr[i]);
 
   printf("Enter the position where you want to delete the element: ");
   scanf("%d", &position);
 
   if (position >= nbr+1)
      printf("Unable to delete.\n");
   else
   {
      for (i = position - 1; i < nbr - 1; i++)
         arr[i] = arr[i+1];
 
      printf("After deletion, the array = ");
 
      for (i = 0; i < nbr - 1; i++)
         printf("%4d", arr[i]);
   }
 
   return 0;
}