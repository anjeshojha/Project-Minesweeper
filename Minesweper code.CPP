#include <iostream.h>
#include <conio.h>
#include <graphics.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//i edited this file to make it better!
union REGS i, o; //declaring mouse registers.
int interval=20;
int row=15;
int col=15;
int c=0 ,  r=0 , bn=0;
int box[15][15]={0};
int box_flag[15][15]={0};
int box_number[15][15]={0};
int x ,y,status=1, button;


int check_bomb()
{
if(box[r-1][c-1]==1)
return 1;

else
return 0;

}

void set_bomb()
{

int mine=0;
	int a , b ;
	for(int i=0; i<15 ;i++)
	for(int j=0;j<15;j++)
		{
		 box[i][j]=0;
		}

	randomize();  //<stdlib.h> , inbuilt function that calls time function i.e.  time.h prototyped
	time_t t ;

	srand((unsigned) time(&t));
	while(1) //infinite loop
	{
	a=rand()%15;
	b=rand()%15;
	   if(box[a][b] != 1)
	   {
	  box[a][b]=1;
	  mine++;
	   }
	if(mine==30) //breaking loop of while.
	{
	   break;
	}
	}
}


void draw_bomb() //check location of bombs.
{   setcolor(RED);
	for(int i=0; i<15 ; i++)
		for(int j=0; j<15 ;j++)
			{
				if(box[i][j]==1)
					outtextxy(160+j*20+10 , 100+i*20+10 , "*");
			}
}


//show mouse
void showmouse(){
 i.x.ax=1;
 int86(0x33, &i , &o);
}

//used to hide mouse pointer
void hidemouse(){
 i.x.ax=2;
 int86(0x33, &i , &o);
}

void set_rcbn(int x , int y) //setting row , column and box number.
{                            // x and y represents the pixels.

c=(x-160)/interval+1;
r=(y-100)/interval+1;
bn=(r-1)*15+c;



}


void drawbox()     //used to draw the box from lines.
{
int interval=20;
int row=15;
int col=15;
int gd=DETECT , gm;
initgraph(&gd , &gm , "");
setcolor(3);

 for(int i=1;i<row;i++)
 {

  line(160,100+i*interval,460,100+i*interval);

 }

 for(int j=1; j<col ; j++)
 {
  line(160+j*interval ,  100  , 160+j*interval , 400);


 }

 rectangle(160 , 100 , 460 ,400);


}

void movemouseptr(int x, int y)   //used to mouse pointer.
{
 i.x.ax=4;
 i.x.cx=x;
 i.x.dx=y;
 int86(0X33, &i, &o);


}
void restrict(int x1, int y1 , int x2, int y2)
{ i.x.ax=7; //horizontal
i.x.cx=x1;
i.x.dx=x2;
int86(0x33,&i,&o);
i.x.ax=8;   //vertical
i.x.cx=y1;
i.x.dx=y2;
int86(0x33,&i,&o);

}


void getmousepos(int *button , int *x , int *y) //used to get mouse positions . i.e. the coordinates of the pixels.
{
i.x.ax=3;
int86(0x33 , &i , &o);
*button = o.x.bx;
*x=o.x.cx;
*y=o.x.dx;
}


void print_box(){         //used to print the value of bombs and nearby boxes.
	for(int i=0; i<15 ; i++)
		for(int j=0; j<15 ;j++)
		{
			  printf("%2d" , box_number[i][j]);
		}

}


void draw_flag(int cl)  //used for flagging.

{

	int i=r-1;
	int j=c-1;
	box_flag[i][j]=1; //setting '1' for flag.
	setcolor(cl);
	hidemouse();
	settextstyle(0,0,0);
	outtextxy(160+j*20+10 , 100+i*20+10 , "F");
	showmouse();
	setcolor(RED);


}


void draw_number(){  //used for debugging.
	char string[10];
	for(int i=0;i<15;i++)
		for(int j=0;j<15;j++){
			if(box_number[i][j] != -1 ) {
				setcolor(YELLOW);
				hidemouse();
				settextstyle(0,0,0);
				outtextxy(160+j*20+10 , 100+i*20+10 , itoa(box_number[i][j], string, 10));
				showmouse();
				setcolor(RED);
			}
		}

}

void set_number()   //used to set the number of nearby bombs in a box! (exactly)
{
	int counter=0;
	for(int i=0;i<15;i++)
		for(int j=0;j<15;j++)
			{
				counter = 0;
				if(box[i][j]==1)
					counter=-1;
				else
				{
				 if((i-1)>=0 && (j-1) >=0)
					if(box[i-1][j-1]==1)
						counter++;
				if((i-1)>=0)
					if(box[i-1][j]==1)
						counter++;

				if((i-1)<15 && (j+1)<15)
					if(box[i-1][j+1]==1)
						counter++;

				if((j-1)>=0)
					if(box[i][j-1]==1)
						counter++;

				if((j+1)<15 )
					if(box[i][j+1]==1)
						counter++;

				if((i+1)<15 && (j-1)>=0)
					if(box[i+1][j-1]==1)
						counter++;

				if((i+1)<15)
					if(box[i+1][j]==1)
						counter++;

				if((i+1)<15 && (j+1)<15)
					if(box[i+1][j+1]==1)
						counter++;
				}
				box_number[i][j] = counter;

			}


}  // end of set_number



void display_box(int i , int j)
{
	char string[10];
		setcolor(15);
					hidemouse();
					settextstyle(0,0,0);
					outtextxy(160+j*20+10 , 100+i*20+10 , itoa(box_number[i][j], string, 10));
					showmouse();




}


//USING MULTIPLE SPREAD FUNCTIONS TO AVOID LAGGING.
//used for spreadling.
void spread(int i , int j)
	{
		if(i>=0 && i<15 && j>=0 && j<15)//  restricting the box boundary.
			 {
				if(box_number[i][j]!=0)
					{
						display_box(i , j);
						return;


					}



		{
			display_box(i , j);
			spread(i-1 , j-1);
			spread(i-1 , j);

		}


	 }


}

void spread1(int i  , int j)
 {
	 if(i>=0 && i<15 && j>=0 && j<15)
		{
			if(box_number[i][j]!=0)
				{
					display_box(i , j);
					return;


		}


	{
		display_box(i, j);
		spread1(i-1 , j+1);
		spread1(i , j-1);


	}

  }

}


void spread2(int i  , int j)
 {
	if(i>=0 && i<15 && j>=0 && j<15)
		{
			 if(box_number[i][j]!=0)
				{
					display_box(i , j);
					return;



				}

	{
		display_box(i, j);
		spread2(i , j+1);
		spread2(i+1 , j-1);


	}

  }

}


void spread3(int i  , int j)
	{
		if(i>=0 && i<15 && j>=0 && j<15)
			{
				if(box_number[i][j]!=0)
					{
						display_box(i , j);
						return;


					}

	{
		display_box(i, j);
		spread3(i+1 , j);
		spread3(i+1 , j+1);



	 }

  }

}



//first_screen used for animation in the beginning
void first_screen()
	{
		int gd=DETECT , gm;
		initgraph(&gd , &gm , "");
		int a=getmaxx();
		int b=getmaxy();
		settextstyle(SMALL_FONT , 0 , 11);
		setcolor(3);
		rectangle(0,0 , a, b);
		rectangle(5,5 , a-5 , b-5);
		for(int i=0; i<650;i++)
		{
			line(0+i , 5 , 10+i ,0);
			i+=5;
		}
		for(int j=0 ; j<650 ; j++)
		{
			line(0+j , b  , 10+j ,b-5);
			j+=5;

		}
		for(int k=0; k<500; k++)
		{
			line(5 , 0+k , 0 , 10+k);
			k+=5;


		}
		for(int l=0; l<500 ; l++)
		{
			line(a-5 , 0+l , a , 10+l);
			l+=5;


		}
		outtextxy(100 , 200 , "M");
		delay(300);
		outtextxy(100 , 200 , "M");
		outtextxy(120 , 200 , "I");
		delay(300);
		outtextxy(120 , 200 , "I");
		outtextxy(140 , 200 , "N");
		delay(300);
		outtextxy(140 , 200 , "N");
		outtextxy(160 , 200 , "E");
		delay(300);
		outtextxy(160 , 200 , "E");
		outtextxy(180 , 200 , "S");
		delay(300);
		outtextxy(180 , 200 , "S");
		outtextxy(200 , 200 , "W");
		delay(300);
		outtextxy(200 , 200 , "W");
		outtextxy(220 , 200 , "E");
		delay(300);
		outtextxy(220 , 200 , "E");
		outtextxy(240 , 200 , "E");
		delay(300);
		outtextxy(240 , 200 , "E");
		outtextxy(260 , 200 , "P");
		delay(300);
		outtextxy(260 , 200 , "P");
		outtextxy(280 , 200 , "E");
		delay(300);
		outtextxy(280 , 200 , "E");
		outtextxy(300 , 200 , "R");
		delay(300);
		outtextxy(300 , 200 , "R");
		outtextxy(320 , 200 , ".");
		delay(600);
		outtextxy(340  , 200 , ".");
		delay(500);
		outtextxy(360 , 200 , ".");
		delay(600);
		outtextxy(380 , 200, ".");
		delay(500);
		outtextxy(400 , 200 , ".");
		delay(400);
		outtextxy(400 , 200 , ".");
		delay(300);
		outtextxy(420 , 200 , ".");
		delay(200);

		for(int v=0 ; v<180; v++)
		{
			outtextxy(420+v , 200 , ".");
			delay(200);
			v+=20;
		}

}

//count_down function is used for animation in the beginning.
void count_down(){
int x , y, i;
cleardevice();

 x=getmaxx()/2;
 y=getmaxy()/2;
 settextstyle(TRIPLEX_FONT,HORIZ_DIR,3);
 setbkcolor(rand());
 setcolor(4);
 int c=150;
for(int e=0;e<15;e++) //Change 15 to your desired value
 {
  for(int i=50;i<100;i++)
  {
	setcolor(3);
	settextstyle(SMALL_FONT, HORIZ_DIR, 10);
	outtextxy(x,y,"Loading...");
	settextstyle(SMALL_FONT, HORIZ_DIR, 5);
	outtextxy(x+5,y+5,"Please Wait... ");
	circle(x,y,i);
	circle(x,y,c);
	c--;
	cleardevice();
  }
  }
  setcolor(6);
  settextstyle(SMALL_FONT, HORIZ_DIR, 10);
  settextjustify(x,y);
  outtextxy(x-70,y-60,"WELCOME");
  delay(150);
  outtextxy(x-70,y-20,"TO");
  delay(150);
  outtextxy(x-10,y-20,"THE");
  delay(150);
  outtextxy(x-70,y+15,"GAME");
  delay(150);

  outtextxy(250,y+50 , "PRESS ENTER ");
  outtextxy(250 , y+80 , "TO START");

}

 void start_game()

{
	settextstyle(3,0,3);
	outtextxy(200,150, "1.Start Game " );
	showmouse();
	restrict(200 , 150 , 350 , 180);
	while(!kbhit())
	{
		getmousepos(&button , &x , &y);
		if(button==1)
			count_down();


	}


}


void options()

{
	int a=getmaxx();
	int b=getmaxy();
	rectangle(0,0 , a, b);
	rectangle(5,5 , a-5 , b-5);
	for(int i=0; i<650;i++)
	{
		line(0+i , 5 , 10+i ,0);
		i+=5;
	}
	for(int j=0 ; j<650 ; j++)
	{
		line(0+j , b  , 10+j ,b-5);
		j+=5;

	}
	 for(int k=0; k<500; k++)
	{
		line(5 , 0+k , 0 , 10+k);
		k+=5;


	}
	for(int l=0; l<500 ; l++)
	{
		line(a-5 , 0+l , a , 10+l);
		l+=5;

   }

	start_game();

}

void additional()

{

	 setcolor(6);
	outtextxy(200 , 410 ," Press Enter to Exit Anytime");

}




//start of main part.
int main()

{

	int gdriver = EGA, gmode = EGAHI, errorcode; //initializing graphics;
	initgraph(&gdriver , &gmode , "");
	first_screen(); //animation part
	cleardevice();
	count_down();   //animation part
	getch();
	setcolor(RED);
	set_bomb();
	set_number();
	drawbox();
	additional();

	//calling mouse function
	if(status == 0)
		printf("Mouse support not available.\n");
	else
	{
		showmouse();
		movemouseptr(100,200); //calling movemouseptr
		restrict(160,100,455,400);//calling restrict function
		setcolor(6);
		settextstyle(3,0,5);
		outtextxy(165,20," MINESWEEPER ");
		setbkcolor(0);

	 }


	while(!kbhit())
	{
		getmousepos(&button , &x , &y);
			if(button==1)
				{
					button=-1;
					set_rcbn(x , y);

		  // outtextxy(10 , 10 , itoa(box[r-1][c-1],string,10));
			if(check_bomb()==1)

				{   hidemouse();
					draw_bomb();
					getch();
					cleardevice();
					settextstyle(3,0,5);
					setcolor(6);
					outtextxy(220,210,"GAMEOVER");
					outtextxy(115,250 , "BETTER LUCK NEXT TIME");
					setbkcolor(0);

				}

			else{
			   // there is no bomb
				int i = r-1;
				int j = c-1;
				if(box_number[i][j] != 0){

					spread(i , j);
					spread1(i , j);
					spread2(i , j);
					spread3(i , j);
				}

				else{
					// spread out
					spread(i , j);
					spread1(i , j);
					spread2(i , j);
					spread3(i , j);

					}
			}
		}

		else if( button == 2 )
		{   delay(250);
			button = -1;
			set_rcbn(x , y);
			if(box_flag[r-1][c-1]==1)
			draw_flag(BLACK);
			else
			draw_flag(YELLOW);
		}
	} // end of while loop
   getch();
	return 0;
} // end of main function
