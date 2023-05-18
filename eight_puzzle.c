#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DIMENTION 3

// the structure of each node
struct node {
   int state[DIMENTION][DIMENTION];
   int zero[2];// zero[0]= row zero[1]=column
   int gn;
   int fn;
   struct node *parent;
};
 // the structure of my queue
struct my_queue {
   struct node *node;
   struct my_queue *next;
};

//printing the current node with below format
//      1   2   3   
//      4   5   6
//      7   8   0

void print_puzzle(struct node *anode)
{
    
    for (int i = 0; i < DIMENTION; i++) {
        for (int j = 0; j < DIMENTION; j++) {
            printf("%d ", anode->state[i][j]);
        }
        printf("\n");
    }
    printf("gn=%d, h(n)=%d, fn=%d\n\n", anode->gn, anode->fn - anode->gn,anode->fn);
}

struct node* make_node(int state[DIMENTION][DIMENTION], int zero[2],int gn, int fn,struct node *parent){
    struct node *current = malloc(sizeof(struct node));
    for (int i = 0; i < DIMENTION; i++) {
        for (int j = 0; j < DIMENTION; j++) {
            current->state[i][j] = state[i][j];
        }
    }
    
    current->parent = parent;
    current->gn =gn;
    current->fn =fn;
    current->zero[0]=zero[0];
    current->zero[1]=zero[1];
    return current;
}



// the first heuristic 
int uniform_cost_search(int state[DIMENTION][DIMENTION]){
    return 0;
}
// the second heuristic 
int misplaced_tile_heuristic(int state[DIMENTION][DIMENTION]){
    int gn = 0;
    // the goal state is:
    int goal_state[DIMENTION][DIMENTION];
    for (int i = 0; i < DIMENTION; i++) {
        for (int j = 0; j < DIMENTION; j++) {
            goal_state[i][j] = j + (DIMENTION*i) + 1;
        }
    }
    // compute the misplaced tiles compared to the computed goal state
    for (int i = 0; i < DIMENTION; i++) {
        for (int j = 0; j < DIMENTION; j++) {
            if(state[i][j]!=0){                  // we do not wwant to count "0"
                if(state[i][j]!=goal_state[i][j]){
                    gn++;
                }
            }
        }
    }
    return gn;
}
// the third heuristic 
int manhatan_distance_heuristic(int state[DIMENTION][DIMENTION]){
    int gn = 0;
    for (int i = 0; i < DIMENTION; i++) {
        for (int j = 0; j < DIMENTION; j++) {
            if (state[i][j] != 0) {              // we do not want to count "0"
                gn += abs(i - ((state[i][j]-1)/DIMENTION)) + abs(j - ((state[i][j]-1)%DIMENTION)); // 
            }
        }
    }
    return gn;
}


// choose between the heuristics
int queueing_function(int algo,int state[DIMENTION][DIMENTION]){
    int gn=0;
    if (algo==1) gn=uniform_cost_search(state);
    else if(algo ==2)  gn=misplaced_tile_heuristic(state);
    else gn=manhatan_distance_heuristic(state);
    return gn;
}


int goal_test(struct node *node){
    //create goal state
    int goal_state[DIMENTION][DIMENTION];
    for (int i = 0; i < DIMENTION; i++) {
        for (int j = 0; j < DIMENTION; j++) {
            goal_state[i][j] = j + (DIMENTION*i) + 1;
        }
    }
    goal_state[DIMENTION-1][DIMENTION-1]=0;
    //compare each elements with goal state
    for (int i = 0; i < DIMENTION; i++) {
        for (int j = 0; j < DIMENTION; j++) {
            
            if (node->state[i][j] != goal_state[i][j]) {
                return 0;
            }
            
        }
    }
    printf("match found\n");
    return 1;
}

//struct puzzle_state *find_solution(int initial_tiles[N][N]){
int main(){
    clock_t start_time, end_time;
    double execution_time;

    int nodes_expanded=0; //expanded nodes
    int depth=0;
    int zero[2];
    int initial[DIMENTION][DIMENTION];
    for (int i = 0; i < DIMENTION; i++) { 
        printf("insert input for the %d row:    ", i);
        for (int j = 0; j < DIMENTION; j++) {
            scanf("%d", &initial[i][j]);
            if (initial[i][j]==0){
                zero[0]=i;
                zero[1]=j;
            }
        }
    }
    int algo=0;
    printf("insert the algorithm number: 1-UCS 2-Astar-misplaced 3-Astar-manhatan:   ");
    scanf("%i", &algo);
    int gn = 0;
    int fn = gn + queueing_function(algo,initial);
    struct node *init = make_node(initial,zero,gn,fn,NULL);// SET PARENT TO NULL becase it is the initial node
    struct node *current = NULL;
    struct node *next = NULL;
    //struct node *successor_state = NULL;

   //print_puzzle(init);
    //int currentF=0;
    struct my_queue *queue = malloc(sizeof(struct my_queue));
    queue->node = init;
    queue->next = NULL; // i dont know what is the next yet
    struct my_queue *next_queue = malloc(sizeof(struct my_queue));
    next_queue->node = NULL;
    next_queue->next = NULL; // i dont know what is the next yet
    struct my_queue *front_node = malloc(sizeof(struct my_queue));
    front_node->node = init;
    front_node->next = NULL; // i dont know what is the next yet
    struct my_queue *bestqueue = malloc(sizeof(struct my_queue));
    bestqueue->node = init;
    bestqueue->next = NULL; // i dont know what is the next yet
    
    struct my_queue *tempqueue = queue;
    //if(queue != NULL){ printf("failure")}
    start_time = clock();
    while (queue != NULL) {

        tempqueue = queue;
        bestqueue = queue;
        
        while(tempqueue != NULL){ 
            if (tempqueue->node->fn < bestqueue->node->fn) {
                bestqueue = tempqueue;
            } 
            tempqueue = tempqueue->next;
        }
        current = bestqueue->node;
        queue = bestqueue->next;
        
        print_puzzle(current);
        
        // Check whether this node is the goal state or not
        if (goal_test(current)==1) {
            end_time = clock();
            execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC * 1000;
            printf("Nodes Expanded: %d\n", nodes_expanded);
            printf("Execution time: %f ms\n", execution_time);
            return 1;
        }
        
        // when the current node is not the goal state we need to generate all the childeren using the valid operators
        for (int dir = 0; dir < 4; dir++) {
            int temp_zero[2];
            temp_zero[0]= current->zero[0];
            temp_zero[1]= current->zero[1];
            if(dir==0) temp_zero[0]--;// move zero to the upper tile
            else if(dir==1) temp_zero[1]++;//move zero to the right tile
            else if(dir==2) temp_zero[0]++; //move zero to the lower tile
            else if(dir==3) temp_zero[1]--; //vove zero to the left tile


            //want to make sure that we dont have a wrong operator (valid operators)
            if (temp_zero[0] < 0 || temp_zero[1] <0) // tiles row and col can not be smaller than 0
                continue;
            if(temp_zero[0] >= DIMENTION || temp_zero[1] >= DIMENTION)// tiles row and col can not be larger than the (Dimention-1)
                continue;

            // here i want to prevent loop  over a parent and its child
            if (current->parent != NULL){
                if(current->parent->zero[0]==temp_zero[0]){
                    if(current->parent->zero[1]==temp_zero[1]){
                        continue;
                    }
                }
            }
            // if operation is valid i will create the child 
            int child[DIMENTION][DIMENTION];
            for (int i = 0; i < DIMENTION; i++) {
                for (int j = 0; j < DIMENTION; j++) {
                    child[i][j]=current->state[i][j];
                }
            }
            //swip the data to build child
            child[current->zero[0]][current->zero[1]]=current->state[temp_zero[0]][temp_zero[1]];
            child[temp_zero[0]][temp_zero[1]]=current->state[current->zero[0]][current->zero[1]];

           
            int child_gn = current->gn + 1;
            int child_fn = child_gn + queueing_function(algo,child);
            // Create the successor state
            next = make_node(child, temp_zero, child_gn, child_fn, current);
            struct my_queue *new_queue = malloc(sizeof(struct my_queue));
            
                new_queue->node = next;

                if (queue == NULL){
                    printf("queue null\n");
                    new_queue->next = NULL;
                    queue = new_queue;
                } 
                else if (next->fn < queue->node->fn) {
                    new_queue->next = queue;
                    queue = new_queue;
                } 
                else {
                    struct my_queue *test = queue;
                    while(test->next != NULL && next->fn >= test->next->node->fn){
                        test = test->next;
                    }
                    
                
                        new_queue->next= test->next;
                        test->next = new_queue;
                        //queue = test;
                

                }
                
        }
        nodes_expanded++;  
        
    }

    end_time = clock();
    execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC * 1000;
    printf("Execution time: %f ms\n", execution_time);
    printf("Nodes Expanded: %d\n", nodes_expanded);
    return 0;
}




