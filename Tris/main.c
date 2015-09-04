#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRIS 3

/*
 7  3  4   5  6
 0 |  |  |  |
 1 |  |  |  |
 2 |  |  |  |
*/

typedef struct
{
    int positionX;
    
    int positionY;
    
    float value;
    
} cell;

struct trisPossibility {
    
    cell possibility[TRIS];
    
    float attack;
    
    float defense;
    
    float totValue;
    
    struct trisPossibility * next;
    
};

struct trisPossibility * firtPossibility;

float battleField[TRIS][TRIS] = {{0,0,0},{0,0,0},{0,0,0}};

int winner;

int checkCompleteFiedl()
{
    int complete = 1;
    
    for (int i = 0; i < TRIS; i++)
    {
        for (int j = 0; j < TRIS; j++)
        {
            if (battleField[i][j] == 0)
            {
                complete = 0;
                
                break;
            }
        }
        
        if (complete == 0)
        {
            break;
        }
    }
    
    return complete;
}

int checkTris(){
    
    int tris = 1;
    float checker = 0;
    
    for (struct trisPossibility * p = firtPossibility ; p!=NULL; p=p->next)
    {
        checker = p->possibility[0].value;
        
        tris = 1;
        
        for (int i = 0; i < TRIS; i++) {
            
            if (checker == 0 || checker != p->possibility[i].value) {
                
                tris = 0;
                
                break;
            }
            
        }
        
        if (tris == 1) {
            
            break;
            
        }
        
    }
    
    if (tris == 0 && checkCompleteFiedl() == 1) {
        
        return 1;
        
    }
    
    if (tris == 0) {
        
        return tris;
        
    } else
    {
        if (checker == 2) {
            
            return 2;
            
        } else
        {
            return 3;
        }
    }
    
}

void removeComplete() {
    
    int complete = 1;
    
    struct trisPossibility * prev = NULL;
    
    for (struct trisPossibility * p = firtPossibility ; p!=NULL; )
    {
        for (int i  =  0; i < TRIS; i++)
        {
            if (p->possibility[i].value == 0)
            {
                complete = 0;
                
                break;
            }
        }
        
        if ( complete == 1)
        {
            if (prev == NULL)
            {
                firtPossibility = p->next;
                free(p);
                p = firtPossibility;
                
            } else {
                
                prev->next = p->next;
                free(p);
                p = prev->next;
                
            }
            
        } else {
            
            prev = p;
            
            p = p->next;
        }
        
        complete = 1;
    }
    
}

void getRows(float battlefield[TRIS][TRIS])
{
    
    struct trisPossibility * possibility;
    
    for (int i = 0; i < TRIS; i++)
    {
        possibility = calloc(1, sizeof(struct trisPossibility));
        
        for (int j = 0; j < TRIS; j++)
        {
            cell singleCell;
            
            singleCell.positionX = j;
            singleCell.positionY = i;
            
            singleCell.value = battleField[i][j];
            
            possibility->possibility[j] = singleCell;
            
        }
        
        possibility->next = firtPossibility;
        
        firtPossibility = possibility;

    }
}


void getColums(float battlefield[TRIS][TRIS])
{
    struct trisPossibility * possibility;
    
    for (int i = 0; i < TRIS; i++)
    {
        possibility = calloc(1, sizeof(struct trisPossibility));
        
        for (int j = 0; j < TRIS; j++)
        {
            cell singleCell;
            
            singleCell.positionX = i;
            singleCell.positionY = j;
            
            singleCell.value = battleField[j][i];
            
            possibility->possibility[j] = singleCell;
            
        }
        
        possibility->next = firtPossibility;
        
        firtPossibility = possibility;
        
    }
}

void getObl(float battlefield[TRIS][TRIS])
{
    struct trisPossibility * possibility;
    
    possibility = calloc(1, sizeof(struct trisPossibility));
    
    for (int i = 0; i < TRIS; i++)
    {
        cell singleCell;
            
        singleCell.positionX = i;
        singleCell.positionY = i;
            
        singleCell.value = battleField[i][i];
            
        possibility->possibility[i] = singleCell;
        
    }
    
    possibility->next = firtPossibility;
    
    firtPossibility = possibility;
    
    possibility = calloc(1, sizeof(struct trisPossibility));
    
    for (int i = 0, j = TRIS-1; i < TRIS; i++, j--)
    {
        cell singleCell;
        
        singleCell.positionX = i;
        singleCell.positionY = j;
        
        singleCell.value = battleField[i][j];
        
        possibility->possibility[i] = singleCell;
        
    }
    
    possibility->next = firtPossibility;
    
    firtPossibility = possibility;
    
}

void updateCells(float battleField[TRIS][TRIS])
{
    for (struct trisPossibility * p = firtPossibility ; p!=NULL; p=p->next)
    {
        float attack = 0, defense = 0;
        
        for (int i = 0; i < TRIS; i++)
        {
            int x = p->possibility[i].positionX;
            int y = p->possibility[i].positionY;
            
            p->possibility[i].value = battleField[x][y];
            
            if (p->possibility[i].value == 1.25)
            {
                attack += 1.25;
            }
            
            if (p->possibility[i].value == 2) {
                
                defense += 2;
                
            }
        }
        
        p->attack = attack;
        
        p->defense = defense;
        
        p->totValue = attack + defense;
        
        attack = 0;
        defense = 0;
    }
}

void evaluate(){
    
    struct trisPossibility * attack = firtPossibility,
        *defense = firtPossibility, * totValue = firtPossibility;
    
    for (struct trisPossibility * p = firtPossibility; p!=NULL; p=p->next)
    {
        if (totValue->totValue < p->totValue)
        {
            totValue = p;
            
        }
    }
    
    for (struct trisPossibility * p = firtPossibility; p!=NULL; p=p->next)
    {
        
        if (totValue->totValue == 2)
        {
            if (attack->attack <= p->attack)
            {
                attack = p;
                
            }
            
        } else {
            if (attack->attack < p->attack)
            {
                attack = p;
                
            }
        }
        
    }
    
    for (struct trisPossibility * p = firtPossibility; p!=NULL; p=p->next)
    {
        if (defense->defense < p->defense)
        {
            defense = p;
            
        }
    }
    
    int positionX = totValue->possibility[0].positionX,
        positionY = totValue->possibility[0].positionY,
        moveDone = 0;
    
    if (defense->defense < 4 && attack->attack < 1.25) {
        
        for (int i = 0; i < TRIS; i++) {
            
            if (totValue->possibility[i].value == 0) {
                
                positionY = totValue->possibility[i].positionY;
                positionX = totValue->possibility[i].positionX;
                
                break;
            }
        }
        
        moveDone = 1;
        
        
    }
    
    if (defense->defense == 4 && attack->attack < 2) {
        
        for (int i = 0; i < TRIS; i++) {
            
            for (int i = 0; i < TRIS; i++) {
                
                if (defense->possibility[i].value == 0) {
                    
                    positionY = defense->possibility[i].positionY;
                    positionX = defense->possibility[i].positionX;
                }
            }
            
        }
        
        moveDone = 1;
        
    }
    
    if (attack->attack <= 2.5 && moveDone != 1) {
        
        for (int i = 0; i < TRIS; i++) {
            
            for (int i = 0; i < TRIS; i++) {
                
                if (attack->possibility[i].value == 0) {
                    
                    positionY = attack->possibility[i].positionY;
                    positionX = attack->possibility[i].positionX;
                }
            }
            
        }
        
    }
    
    battleField[positionX][positionY] = 1.25;
    
}

int getPlayer()
{
    int player;
    
    player = rand() % 2;
    
    return player + 1;
}

void printFiled(float battleField[TRIS][TRIS])
{
    for (int i = 0; i < TRIS; i++)
    {
        for (int j = 0; j < TRIS; j++)
        {
            if (battleField[i][j] == 1.25)
            {
                
                if (j == TRIS-1)
                {
                    printf("| O |\n");
                    
                } else {
                    
                    printf("| O ");
                }
            
            }
            
            if (battleField[i][j] == 2)
            {
                if (j == TRIS-1)
                {
                    printf("| X |\n");
                    
                } else {
                    
                    printf("| X ");
                }
            }
            
            if (battleField[i][j] == 0)
            {
                if (j == TRIS-1)
                {
                    printf("|   |\n");
                    
                } else {
                    
                    printf("|   ");
                }
            }
        }
    }
    
    printf("\n");
    
}

void cleanBattleField (float battleField[TRIS][TRIS]) {
    
    for (int i = 0; i < TRIS; i++) {
        
        for (int j = 0; j < TRIS; j++) {
         
            battleField[i][j] = 0;
            
        }
    }
    
}

void getPlayerMove()
{
    printf("please write your move, x and y cordinate \n");
    
    printf("ex: 0 - 2\n");
    
    int x, y;
    
    scanf("%d - %d", &x, &y);
    
    while (1) {
       
        if (x >= 0 && x < TRIS && y >= 0 && y < TRIS)
        {
            break;
        }
        
        if (x < 0 || x >= TRIS)
        {
            printf("please write corrct x\n");
            
            scanf("%d", &x);
        }
        
        if (y < 0 || y >= TRIS)
        {
            printf("please write corrct y\n");
            
            scanf("%d", &y);
        }
        
    }
    
    battleField[x][y] = 2;
}


int firstMoveUman()
{
    int result;
    
    while (1) {
        
        getPlayerMove();
        
        updateCells(battleField);
        
        result = checkTris();
        
        if (result) return result;
        
        removeComplete();
        
        evaluate();
        
        updateCells(battleField);
        
        printFiled(battleField);
        
        result = checkTris();
        
        if (result) return result;
        
    }
    
}

int firstMovePc()
{
    int bingooo[2] = {0,2}, x, y;
    
    x = bingooo[rand()%2];
    y = bingooo[rand()%2];
    
    battleField[x][y] = 1.25;
    
    updateCells(battleField);
    
    printFiled(battleField);
    
    int result;
    
    while (1) {
        
        getPlayerMove();
        
        updateCells(battleField);
        
        result = checkTris();
        
        if (result) return result;
        
        removeComplete();
        
        evaluate();
        
        updateCells(battleField);
        
        printFiled(battleField);
        
        result = checkTris();
        
        if (result) return result;
        
    }
    
}


void playGame(int firtsGame)
{
    int player, result;
    
    if (firtsGame == 1)
    {
        player = getPlayer();
        
    }else
    {
        player = winner;
    }
    
    cleanBattleField(battleField);
    
    getRows(battleField);
    getColums(battleField);
    getObl(battleField);
    
    if ( player == 1 )
    {
        result = firstMoveUman();
        
    } else {
        
        result = firstMovePc();
    }
    
    if (result == 1) printf("No Winner\n");
    
    if (result == 2)
    {
        winner = 1;
        
        printf("the winner is Player 1\n");
    }
    
    if (result == 3)
    {
        winner = 2;
        
        printf("the winner is Player 2\n");
    }
    
}

int main(int argc, const char * argv[])
{
    time_t t;
    
    srand((unsigned) time(&t));
    
    char ch;
    
    int firstGame = 1;
    
    printf("Would you like to star a new game? type Y or n to exit\n");
    
    while (1)
    {
        ch = getchar();
        
        if (ch == 'Y' || ch == 'y')
        {
            printf("Loading................\n");
            
            playGame(firstGame);
            
            firstGame = 0;
            
            printf("would you like to play again?? \n");
            
        }
        
        if (ch == 'N' || ch == 'n')
        {
            printf("Goodbye\n");
            
            return 0;
        }
    }
        
    return 0;
}
