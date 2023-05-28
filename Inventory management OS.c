#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>

// Declare a shared data structure to represent the artist's inventory
typedef struct
{
    int vase_count;    // number of vases in stock
    pthread_mutex_t lock;
} inventory_t;

// Declare semaphores to manage the availability of the vase
sem_t vase_semaphore;


inventory_t artist_inventory;
inventory_t market_inventory;
inventory_t online_inventory;


void init_inventory(inventory_t *inventory, int count)
{
    inventory->vase_count = count;
    pthread_mutex_init(&inventory->lock, NULL);
}


void visit_market()
{


    pthread_mutex_lock(&artist_inventory.lock);

    if (market_inventory.vase_count > 0)
    {
        sem_wait(&vase_semaphore);
        printf("\n\nAlice finds the vase at the market but decides not to buy it.\n");

        sem_post(&vase_semaphore);
    }
    else
    {

        printf("\nThe vase is not available at the market.\n");
    }
    pthread_mutex_unlock(&artist_inventory.lock);


}


void search_online()
{
    // Check if the vase is available in the online inventory
    pthread_mutex_lock(&artist_inventory.lock);
    if (online_inventory.vase_count > 0)
    {
        printf("\nAlice finds the vase online and decides to buy it.\n");

        // Decrement the vase semaphore to indicate that the vase is no longer available
        sem_wait(&vase_semaphore);


        online_inventory.vase_count--;
        artist_inventory.vase_count--;
        sem_post(&vase_semaphore);

        printf("\nAlice has successfully purchased the vase online.\n");


        printf("\n\nAfter Details : \n");
        printf("Item in Artist's inventory : %d\n",artist_inventory.vase_count);
        printf("Item in market's inventory : %d\n",market_inventory.vase_count);
        printf("Item in online's inventory : %d\n",online_inventory.vase_count);


    }
    else
    {
        printf("The vase is not available online.\n");
    }
    pthread_mutex_unlock(&artist_inventory.lock);

}


void make_purchase()
{

    pthread_mutex_lock(&artist_inventory.lock);
    if (market_inventory.vase_count > 0)
    {
        printf("Alice decides to buy the vase at the market.\n");


        sem_wait(&vase_semaphore);

        // Update the market inventory to reflect the purchase
        market_inventory.vase_count--;
        artist_inventory.vase_count--;
        sem_post(&vase_semaphore);
        printf("Alice has successfully purchased the vase from market.\n");

        printf("\n\nAfter Details : \n");
        printf("Item in Artist's inventory : %d\n",artist_inventory.vase_count);
        printf("Item in market's inventory : %d\n",market_inventory.vase_count);
        printf("Item in online's inventory : %d\n",online_inventory.vase_count);


    }


    pthread_mutex_unlock(&artist_inventory.lock);

}

int main()
{
    pid_t pid1, pid2, pid3;

    int ai,mi,oi;


    // Initialize the vase semaphore
    sem_init(&vase_semaphore, 0, 1);

    printf("Initial Inventory details : \n");
    printf("Item in Artist's inventory : ");
    scanf("%d",&ai);
    printf("\nItem in market's inventory : ");
     scanf("%d",&mi);
    printf("\nItem in online's inventory : ");
     scanf("%d",&oi);
   printf("\n");
  // Initialize the shared data structure
    init_inventory(&artist_inventory, ai);
    init_inventory(&market_inventory, mi);
    init_inventory(&online_inventory, oi);


    int x;

    while(1)
    {

     printf("\n1.Alice did not decide to buy.");
    printf("\n2.Alice will buy from market.");
     printf("\n3.Alice will buy from online.");
      printf("\n4.Exit.\n");
     printf("Enter your choice :");
    scanf("%d",&x);


    switch(x)
    {
        case 1:
             pid1 = fork();
             if (pid1 == 0)
            {
                visit_market();

            }
                break;

         case 2:
                pid2 = fork();
                if (pid2 == 0)
                {
                   make_purchase();

                }
                break;
        case 3:
               pid3 = fork();
                if (pid3 == 0)
                 {

                     search_online();

                }
                break;
        case 4:
               exit(0);
       default:
               printf("Wrong input try again.\n");


    }








    // Wait for all child processes to complete

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);
    pthread_mutex_destroy(&artist_inventory.lock);

}

    return 0;

}
