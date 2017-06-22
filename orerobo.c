/* robots */
#include <stdio.h>
#include <stdlib.h>

#define MAP_SIDE 60     /*X*/
#define MAP_LENGTH 20   /*Y*/

struct position {
  int x;
  int y;
  int judge;     /*0:robot,1:scrap*/
};


int min(int i,int j);
void map_create(struct position *hero,struct position robot[40]);
void map_display(void);
void hero_teleport(struct position *hero);
int hero_move(struct position *hero,int command);
int enemy_move(struct position *hero,struct position robot[40]);
int commander(int command,struct position *hero);
void scrap_work(struct position robot[40]);
void clear_judge(struct position robot[40],struct position *hero);


int map[MAP_SIDE][MAP_LENGTH];
int lv=1;
int score=0;


int min(int i,int j){
  if(i>j) return j;
  else return i;
}

void map_create(struct position *hero,struct position robot[40])  
{
  int i,j,randS,randL;
  int robots;
  for(i=0;i<MAP_LENGTH;i++){
    for(j=0;j<MAP_SIDE;j++){
      map[i][j]= 0;  /*0:empty*/
    }
  }
  
  srandom(0);
   
  robots = min(lv*5,40);
  
  for(i=0;robots>i;i++){
    randS=random()%MAP_SIDE;
    randL=random()%MAP_LENGTH;
    if(map[randS][randL]!=2){
      robot[i].x=randS;
      robot[i].y=randL;
      map[randS][randL] = 2;  /*2:robot*/
      robot[i].judge=0;
    }else{
      i--;
    }
  }
  i=0;
  while(i==0){
    randS=random()%MAP_SIDE;
    randL=random()%MAP_LENGTH;
    if(map[randS][randL]!=2){
      hero->x=randS;
      hero->y=randL;
      map[hero->x][hero->y] = 1;  /*1:hero*/
         i=1;
    }
  }
}
  
void map_display(void)
{
  int i,j;

  for(i=0;i<MAP_LENGTH+1;i++){
    if(i==0) printf("+------------------------------------------------------------+");
    else{
      for(j=0;j<MAP_SIDE+2;j++){
	if(j==0 || j==MAP_SIDE+1) printf("|");
	else{
	  if(map[j-1][i-1]==0) printf(" "); 
	  if(map[j-1][i-1]==1) printf("@");                       /*hero*/
	  if(map[j-1][i-1]==3) printf("*");                       /*scrap*/
	  if(map[j-1][i-1]==2) printf("+");                       /*robot*/
	}
      }
    }
    printf("\n");
  }
  printf("+------------------------------------------------------------+\n");
}

void hero_teleport(struct position *hero)
{
  int randS,randL,i;
  i=0;
  while(i==0){
    randS=rand()%MAP_SIDE;
    randL=rand()%MAP_LENGTH;
    if(map[randS][randL]!=2 && map[randS][randL]!=1){
      map[hero->x][hero->y] = 0;
      hero->x=randS;
      hero->y=randL;
      map[hero->x][hero->y] = 1;  /*1:hero*/
      i=1;
    }  
  }
}

int enemy_move(struct position *hero,struct position robot[40])
{
  int i,dummy_x,dummy_y;
  
  for(i=0;i<min(lv*5,40);i++){
    if(robot[i].judge==0){
       
      dummy_x=robot[i].x;
      dummy_y=robot[i].y;
    
      if(hero->x < robot[i].x) robot[i].x--;
      else if(hero->x > robot[i].x) robot[i].x++;

      if(hero->y < robot[i].y) robot[i].y--;
      else if(hero->y > robot[i].y) robot[i].y++;

      map[dummy_x][dummy_y] = 0;
      
    
      if(map[robot[i].x][robot[i].y]==1) return 1;
      else map[robot[i].x][robot[i].y]=2;
    }
  }
  return 0;
}

void scrap_work(struct position robot[40])
{
  int i,j;
  for(i=0;i<min(lv*5,40);i++){
    for(j=0;j<min(lv*5,40);j++){
      if(i!=j){
	if(robot[i].x==robot[j].x && robot[i].y==robot[j].y){
	  map[robot[i].x][robot[j].y]=3;
	  score += (robot[i].judge+1)%2 + (robot[j].judge+1)%2;
	  robot[i].judge=1;
	  robot[j].judge=1;
	}
      }
    }
  }
}

void clear_judge(struct position robot[40],struct position *hero)
{
  int i;
  for(i=0;robot[i].judge==1;i++){}
  if(i==min(lv*5,40)){
    score += lv*10;
    lv++;
    printf("level%d clear!\n",lv);
    map_create(hero,robot);
  }
}

int commander(int command,struct position *hero)
{
  int f;
  if(command==0){
    hero_teleport(hero);
  }else if(command>0 && command<10){
    f = hero_move(hero,command);
    return f;
  }
  
  return 0;
}

int hero_move(struct position *hero,int command)
{
  int dummy_x,dummy_y,f;

  dummy_x=hero->x;
  dummy_y=hero->y;
    
  if(command==7 && hero->x!=0 && hero->y!=0){ 
    hero->x--;
    hero->y--;
  }if(command==8 && hero->y!=0){
    hero->y--;
  }if(command==9 && hero->x!=MAP_SIDE-1 && hero->y!=0){
    hero->x++;
    hero->y--;
  }if(command==4 && hero->x!=0){
    hero->x--;
  }if(command==6 && hero->x!=MAP_SIDE-1){
    hero->x++;
  }if(command==1 && hero->x!=0 && hero->y!=MAP_LENGTH-1){
    hero->x--;
    hero->y++;
  }if(command==2 && hero->y!=MAP_LENGTH-1){
    hero->y++;
  }if(command==3 && hero->x!=MAP_SIDE-1  && hero->y!=MAP_LENGTH-1){
    hero->x++;
    hero->y++;
  }
  
    if(map[hero->x][hero->y]==2) return 1;
    else if(map[hero->x][hero->y]==3) return 0;
    else{
      map[hero->x][hero->y] = 1;
      if(dummy_x!=hero->x || dummy_y!=hero->y) map[dummy_x][dummy_y] = 0;
      return 0;
    }
}

int main(void)
{
  struct position *hero;
  struct position robot[40];
  
  int flag=0;
  int command=0;
  
  map_create(hero,robot);
  
  while(flag==0){
    map_display();
    printf("(level:%d score:%d):?",lv,score);
    scanf("%d",&command);
    flag=commander(command,hero);
    flag=enemy_move(hero,robot);
    scrap_work(robot);
    clear_judge(robot,hero);
    }
  printf("gameover!\n");
  
  return 0;
}
