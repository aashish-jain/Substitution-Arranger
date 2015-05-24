/////////////////////////////---------------------\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//===========================Substitution Arranger==========================\\\
/*\\\\\\\\\\\\\\\\\\\\\\\\\\\---------------------////////////////////////////*/

#include <conio.h>
#include <ctype.h>
#include <dos.h>
#include <fstream.h>
#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ENTER 13
#define UP 72
#define DOWN 80
#define RIGHT 75
#define LEFT 77
#define ESC 27
#define SPACE 32
#define NUM8 8
#define NUM2 2
#define BACKSPACE 8
#define CHNULL 0

#define NEW_FONT 7
#define ITALICS_FONT 5

fstream file,f;

int dd=-1,mm=-1,yy=-1,dow=-1,tub=0;

char day[7][10]={"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday",};
char masterfile[]="data\\database.dat",
     tempfile[]="data\\temp.dat",
     subfile[]="data\\substitution.dat",
     htufile[]="c:\\tc\\bin\\sub\\htu.txt";//Change it to the directory where the htu file is kept

void init_graph();
void initializing();
void loading();
void options(char*,char*,char*,char*,char*,char*);
menu();
void substitution();
void database();
void htu();
void about();
int check(char *);
set_tptr();
void bubble_sort();
void arr_substitute();
void view_substitute();
void m_list();
void sub_list();
void db_list();
void fread();
void fwrite();
void fmodify();
void fdelete();
int  getnum(int,int,int);
char *getstr(int,int,int);
char *getalnum(int,int,int);
f_asso(fstream &);
void set_date();
void set_dayofwk();

class timetable
{
 public:
 char p[5][8][4];
 timetable();
};

timetable::timetable()
{
 for(int i=0;i<5;i++)
  for(int j=0;j<8;j++)
   p[i][j][0]=CHNULL;
}

class teacher : public timetable
{
 int code;
 char name[11];
 protected:
 char subject[11],clt[4];
 int leaves,l;
 public:
 int nop[5];

 teacher();
 int getcode();
 void table();
 int constraints();
 int accept(int);
 void display();

 rtn_code()
 {
  return code;
 }

 char * rtn_name()
 {
  return name;
 }

 void onleave()
 {
  l=1;
 }

 int ispresent()
 {
  return !l;
 }

}t,*tptr=CHNULL;

teacher::teacher()
{
 code=leaves=l=0;
 name[0]=subject[0]=clt[0]=CHNULL;
 nop[0]=nop[1]=nop[2]=nop[3]=nop[4]=0;
}

int teacher::getcode()
{
 teacher _t;
 file.close();
 file.open(masterfile,ios::in);
 if(f_asso(file))
 {
  file.seekg(0,ios::end);
  if(file.tellg()!=0)
  {
   file.seekg((long)-1*(long)sizeof(_t),ios::cur);
   file.read((char*)&_t,sizeof(_t));
   code=_t.rtn_code()+1;
  }
  else
   code=1;
 }
 file.close();
 file.open(masterfile,ios::app|ios::binary);
 return f_asso(file);
}

void teacher::table()
{
 char days[5][4]={"MON","TUE","WED","THU","FRI"};
 char periods[10][2]={"1","2"," ","3","4","5"," ","6","7","8"};
 settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
 outtextxy(225,10,"TIME TABLE");
 settextstyle(DEFAULT_FONT,VERT_DIR,2);
 outtextxy(225,215,"Reccess");
 outtextxy(425,225,"Lunch");
 settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
 outtextxy(50,30,"Teacher code    :");
 outtextxy(350,30,"Teacher name    :");
 outtextxy(50,50,"Subject         :");
 outtextxy(350,50,"Class teacher of:");
 outtextxy(50,70,"Leaves taken    :");
 setcolor(MAGENTA);
 outtextxy(50,40,"(Auto generated)");
 outtextxy(350,40,"(Characters)");
 outtextxy(50,60,"(Characters)");
 outtextxy(350,60,"(Alphanumeric)");
 outtextxy(50,80,"(Integers)");
 setcolor(WHITE);
 //Tabular Column
 for(int i=50;i<650;i+=50)
  line(i,100,i,400);
 line(200,100,250,100);
 line(200,400,250,400);
 line(400,100,450,100);
 line(400,400,450,400);
 for(i=100;i<450;i+=50)
 {
  line(50,i,200,i);
  line(250,i,400,i);
  line(450,i,600,i);
 }
 //Period
 i=0;
 for(int j=120;i<10;i++,j+=50)
  outtextxy(j,115,periods[i]);
 //Day
 for(j=165,i=0;i<5;i++,j+=50)
  outtextxy(65,j,days[i]);
}

int teacher::constraints()
{
 int flag=0,i,j,tp=0;
 for(i=0;i<5;i++)
   tp+=nop[i];
 if(tp==0)
  flag=1;
 if(name[0]==CHNULL)
  flag=1;
 if(subject[0]==CHNULL)
  flag=1;
 if(flag)
 {
  cleardevice();
  outtextxy(150,140,"One of the following constraints voilated!!");
  outtextxy(250,160,"*Teacher name may not be given");
  outtextxy(250,180,"*Subject may not be specified");
  outtextxy(250,200,"*No period is assigned to the teacher");
  getch();
  return 1;
 }
 return 0;
}

int teacher::accept(int var=0)
{
 cleardevice();
 table();
 for(int i=0;i<5;i++)
  nop[i]=0;
 code=var;
 if(code==0)
  getcode();
 char temp[5];
 itoa(code,temp,10);
 outtextxy(190,30,temp);
 strcpy(name,getstr(10,490,30));
 name[10]='\0';
 strcpy(subject,getstr(10,190,50));
 subject[10]='\0';
 strcpy(clt,getalnum(3,490,50));
 clt[3]='\0';
 leaves=getnum(3,190,70);
 int y;
 for(i=0,y=175;i<5;i++,y+=50)
  for(int j=0,x=105;j<8;j++,x+=50)
  {
   if(x==205 || x==405)
    x+=50;
   strcpy(p[i][j],getalnum(3,x,y));
   if(p[i][j][0]!=CHNULL)
    nop[i]++;
  }
  if(constraints())
   return 0;
  return check("Save changes made?");
}

void teacher::display()
{
 cleardevice();
 table();
 char temp[5];
 itoa(code,temp,10);
 outtextxy(186,30,temp);
 outtextxy(486,30,name);
 outtextxy(186,50,subject);
 outtextxy(486,50,clt);
 itoa(leaves,temp,10);
 outtextxy(186,70,temp);
 for(int i=0,y=175;i<5;i++,y+=50)
  for(int j=0,x=105;j<8;j++,x+=50)
  {
   if(x==205 || x==405)
    x+=50;
   outtextxy(x,y,p[i][j]);
  }
  getch();
}

struct substitute
{
 int code,valid,d,m,y;
 char name[11],t_name[8][11],p[8][4],dol[10];

 substitute();
 accept();
 void display();
 void arr_substitute();

 int isvalid()
 {
  return valid;
 }

}*sub,s;

substitute::substitute()
{
 code=0;
 name[0]=CHNULL;
 for(int i=0;i<8;i++)
  t_name[i][0]=p[i][0]=CHNULL;
 d=dd;
 m=mm;
 y=yy;
 code=valid=0;
 strcpy(dol,day[dow]);
}

substitute::accept()
{
 cleardevice();
 settextstyle(TRIPLEX_FONT,HORIZ_DIR,1);
 outtextxy(120,100,"Enter name of the teacher( on leave )");
 settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
 strcpy(name,getstr(11,160,200));
 for(int i=0;i<tub;i++)
  if(!strcmpi(name,tptr[i].rtn_name()))
  {
   valid=1;
   tptr[i].onleave();
   code=tptr[i].rtn_code();
   for(int m=0;m<8;m++)
    strcpy(p[m],tptr[i].p[dow][m]);
   break;
  }
 if(!valid)
 {
  cleardevice();
  settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
  outtextxy(200,240,"Teacher with given name doesn't exist");
   getch();
  return 0;
 }
 return 1;
}

void substitute::display()
{
 cleardevice();
 settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
 char temp[5];
 itoa(code,temp,10);
 temp[0]=CHNULL;
 char temp2[10];
 itoa(d,temp2,10);
 strcat(temp2,"/");
 itoa(m,temp,10);
 strcat(temp2,temp);
 strcat(temp2,"/");
 itoa(y,temp,10);
 strcat(temp2,temp);
 outtextxy(220,25,"Substitution Chart:");
 outtextxy(85,50,"Date:");
 outtextxy(120,50,temp2);
 outtextxy(250,50,"Day:");
 outtextxy(305,50,dol);
 outtextxy(85,75,"Teacher code:");
 outtextxy(190,75,temp);
 outtextxy(250,75,"Name:");
 outtextxy(305,75,name);
 //horizontal lines
 line(85,95,455,95);
 line(85,125,455,125);
 line(85,410,455,410);
 //verical
 line(85,95,85,410);
 line(145,95,145,410);
 line(295,95,295,410);
 line(455,95,455,410);
 outtextxy(90,105,"Period");
 outtextxy(200,105,"Class");
 outtextxy(330,105,"Susbstitute");
 for(int i=0;i<8;i++)
 {
  itoa(i+1,temp,10);
  outtextxy(100,i*32+150,temp);
  outtextxy(200,i*32+150,p[i]);
  outtextxy(330,i*32+150,t_name[i]);
 }
 getch();
}

void substitute::arr_substitute()
{
 bubble_sort();//sort the records on the basis of their free periods
 int i=-1,j,flag;
 if(!valid)
  return;
 while(i<7)
 {
  ++i;
  flag=1;
  //i corresponds to period number while j corresponds to no. of teachers
  while(p[i][0]==CHNULL && i<7)
   ++i;
  if(i==7 && p[7][0]==CHNULL)
   break;
  for(j=0;j<tub;j++)
   if(tptr[j].ispresent() && tptr[j].p[dow][i][0]==CHNULL)//present teacher has a free period
   {
    strcpy(t_name[i],tptr[j].rtn_name());
    strcpy(tptr[j].p[dow][i],"*");
    flag=0;
   }
  if(flag)
   strcpy(t_name[i],"Self-Study");
 }
}

void init_graph()
{
 int gm=DETECT,gd,err;
 initgraph(&gm,&gd,"C:\\tc\\bgi");
 err = graphresult();
 if (err != grOk)
 {
  cout<<"Graphics error:"<< grapherrormsg(err)<<endl;
  cout<<"Press any key to halt:";
  getch();
  exit(1);
 }
 if(getmaxx()!=639 || getmaxy()!=479)
 {
  gotoxy(20,15);
  cout<<"Resolution not compatible with your device";
  gotoxy(35,20);
  cout<<"Quiting...";
  delay(3000);
  closegraph();
 }
 setbkcolor(BLUE);
 setcolor(WHITE);
}

void initializing()
{
 cleardevice();
 int poly1[]={0,200,150,200,200,150,450,150,500,200,640,200},
     poly2[]={0,220,150,220,200,270,450,270,500,220,640,220},pos;
 char *init="Initializing",
      msg[][51]={
		"Connecting to server","Unable to connect!",
		"Trying to reconnect",
		"Unable to connect again!!",
		"Starting in offline mode"};

 pos=280-textwidth(init)/2;
 drawpoly(6,poly1);
 line(0,210,640,210);
 drawpoly(6,poly2);
 rectangle(198,220,442,260);
 setfillstyle(SOLID_FILL,WHITE);
 char temp[2];
 for(int x=200;x<440;x+=22)
 {
  delay(100);
  rectangle(x,222,x+20,258);
  floodfill(x+2,224,WHITE);
  settextstyle(4,0,5);
  temp[0]=*(init++);
  temp[1]=CHNULL;
  outtextxy(pos,160,temp);
  pos+=textwidth(temp);
 }
 temp[0]=*(init++);
 temp[1]=CHNULL;
 delay(100);
 outtextxy(pos,160,temp);
 delay(100);
 settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
 for(x=300,pos=0;x<425;x+=25,pos++)
 {
  outtextxy(320-textwidth(msg[pos])/2,x,msg[pos]);
  if(pos==4)
   delay(1000);
  delay(200);
 }
 cleardevice();
 rectangle(3,1,637,62);
 rectangle(5,3,635,60);
 settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
 outtextxy(150,400,"By robustsoft C 2012-2013 all rights reserved");
 circle(265,403,5);
 outtextxy(610,225,"R");
 circle(613,227,5);
 settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
 outtextxy(202,360,"Press any key to continue");
 settextstyle(GOTHIC_FONT,HORIZ_DIR,7);
 outtextxy(35,200,"Substituion Arranger");
 settextstyle(4,HORIZ_DIR,3);
 outtextxy(310,280,"C++");
 settextstyle(ITALICS_FONT,HORIZ_DIR,1);
 outtextxy(10,10,"Warning!: This computer may be protected by international laws and treaties");
 getch();
}

void loading()
{
 cleardevice();
 rectangle(10,450,634,460);
 for(int i=12;i<=630;i+=10)
 {
  delay(5);
  rectangle(i,452,i+10,458);
  floodfill(i+1,454,WHITE);
 }
 cleardevice();
}

void options(char *header="\0",char *l1="\0",char *l2="\0",
	      char *l3="\0",char * l4="\0",char *l5="\0")
{
 char *project="Susbstitutiton Arranger";
 cleardevice();
 rectangle(3,3,637,477);
 rectangle(5,5,635,475);
 rectangle(8,8,632,472);
 rectangle(135,97,490,400);
 settextstyle(GOTHIC_FONT,HORIZ_DIR,5);
 outtextxy(320-textwidth(project)/2,40,project);
 settextstyle(GOTHIC_FONT,HORIZ_DIR,3);
 outtextxy(320-textwidth(header)/2,100,header);
 line(135,140,490,140);
 settextstyle(GOTHIC_FONT,HORIZ_DIR,3);
 outtextxy(320-textwidth(l1)/2,150,l1);
 outtextxy(320-textwidth(l2)/2,200,l2);
 outtextxy(320-textwidth(l3)/2,250,l3);
 outtextxy(320-textwidth(l4)/2,300,l4);
 outtextxy(320-textwidth(l5)/2,350,l5);
 settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
 outtextxy(150,410,"By robustsoft C 2012-2013 all rights reserved");
 circle(265,413,5);
}

int menu()
{
 cleardevice();
 char ch;
 int y=150;
 m_list();
 rectangle(150,y,475,y+30);
 while(1)
 {
  ch=getch();
  if(ch==NUM8||ch==UP)
  {
   setcolor(BLUE);
   rectangle(150,y,475,y+30);
   setcolor(WHITE);
   if(y>150)y-=50;
   else
   y=350;
   rectangle(150,y,475,y+30);
  }
  else if(ch==NUM2||ch==DOWN)
  {
   setcolor(BLUE);
   rectangle(150,y,475,y+30);
   setcolor(WHITE);
   if(y<350)y+=50;
   else y=150;
   rectangle(150,y,475,y+30);
  }
  else if(ch==ESC)
   return 0;
  else if(ch==SPACE||ch==ENTER)
   switch(y)
   {
    case 150:substitution();
	     m_list();
	     rectangle(150,y,475,y+30);
	     break;
    case 200:database();
	     m_list();
	     rectangle(150,y,475,y+30);
	     break;
    case 250:loading();
	     htu();
	     m_list();
	     rectangle(150,y,475,y+30);
	     break;
    case 300:loading();
	     about();
	     m_list();
	     rectangle(150,y,475,y+30);
	     break;
    case 350:loading();
	     if(check("Quit the program?"))
	     {
	      cleardevice();
	      settextstyle(GOTHIC_FONT,HORIZ_DIR,3);
	      outtextxy(125,200,"Thank you for using Substitution Arranger");
	      getch();
	      return 0;
	     }
	     return 1;
   }
 }
}

void substitution()
{
 cleardevice();
 char ch;
 int y1=150;
 sub_list();
 rectangle(150,y1,475,y1+30);
 while(1)
 {
  ch=getch();
  if(ch==NUM8||ch==UP)
  {
   setcolor(BLUE);
   rectangle(150,y1,475,y1+30);
   setcolor(WHITE);
   if(y1>150)y1-=50;
   else y1=250;
   rectangle(150,y1,475,y1+30);
  }
  else if(ch==NUM2||ch==DOWN)
  {
   setcolor(BLUE);
   rectangle(150,y1,475,y1+30);
   setcolor(WHITE);
   if(y1<250)y1+=50;
   else y1=150;
   rectangle(150,y1,475,y1+30);
  }
  else if(ch==ESC)
   return;
  else if(ch==SPACE||ch==ENTER)
   switch(y1)
   {
    case 150:loading();
	     arr_substitute();
	     sub_list();
	     rectangle(150,y1,475,y1+30);
	     break;
    case 200:loading();
	     view_substitute();
	     sub_list();
	     rectangle(150,y1,475,y1+30);
	     break;
    case 250:return;
   }
 }
}

void database()
{
 cleardevice();
 char ch;
 int y2=150;
 db_list();
 rectangle(150,y2,475,y2+30);
 while(1)
 {
  ch=getch();
  if(ch==NUM8||ch==UP)
  {
   setcolor(BLUE);
   rectangle(150,y2,475,y2+30);
   setcolor(WHITE);
   if(y2>150)y2-=50;
   else y2=350;
   rectangle(150,y2,475,y2+30);
  }
  else if(ch==NUM2||ch==DOWN)
  {
   setcolor(BLUE);
   rectangle(150,y2,475,y2+30);
   setcolor(WHITE);
   if(y2<350)y2+=50;
   else y2=150;
   rectangle(150,y2,475,y2+30);
  }
  else if(ch==ESC)
   return;
  else if(ch==SPACE||ch==ENTER)
   switch(y2)
   {
    case 150:loading();
	     fwrite();
	     db_list();
	     rectangle(150,y2,475,y2+30);
	     break;
    case 200:loading();
	     fread();
	     db_list();
	     rectangle(150,y2,475,y2+30);
	     break;
    case 250:loading();
	     fmodify();
	     db_list();
	     rectangle(150,y2,475,y2+30);
	     break;
    case 300:loading();
	     fdelete();
	     db_list();
	     rectangle(150,y2,475,y2+30);
	     break;
    case 350:return;
    }
 }
}


void htu()
{
 gotoxy(1,1);
 file.open(htufile,ios::in);
 if(!f_asso(file))
  return;
 char w;
 while(file.get(w))
  cout<<w;
 file.close();
 getch();
}

void about()
{
 rectangle(3,3,637,477);
 rectangle(5,5,635,475);
 rectangle(8,8,632,472);
 settextstyle(7,HORIZ_DIR,5);
 outtextxy(90,50,"Substitution Arranger");
 settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
 outtextxy(570,55,"R");
 circle(573,57,5);
 settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
 outtextxy(300,100,"v 1.1");
 outtextxy(380,120,"by robustsoft");
 settextstyle(SANS_SERIF_FONT,HORIZ_DIR,2);
 outtextxy(278,170,"Platform");
 outtextxy(174,250,"Designed & Programmed by");
 outtextxy(241,370,"c 2011-2020");
 circle(245,386,6);
 outtextxy(226,410,"All rights reserved");
 settextstyle(NEW_FONT,HORIZ_DIR,2);
 outtextxy(299,210,"C++");
 outtextxy(280,290,"Aashish");
 getch();
}

int check(char *str)
{
 cleardevice();
 char ch,pch=1;
 rectangle(270,235,320,255);
 int yes=270,no=335;
 rectangle(95,195,550,270);
 outtextxy(320-textwidth(str)/2,200,str);
 outtextxy(280,240,"Yes");
 outtextxy(360,240,"No");
 while(1)
 {
  ch=getch();
  if(ch==RIGHT)
  {
   rectangle(yes,235,yes+50,255);
   pch=1;
   setcolor(BLUE);
   rectangle(no,235,no+50,255);
  }
  else if(ch==LEFT)
  {
   rectangle(no,235,no+50,255);
   pch=0;
   setcolor(BLUE);
   rectangle(yes,235,yes+50,255);
  }
  else if(ch==ENTER)
  {
   setcolor(WHITE);
   return pch;
  }
  setcolor(WHITE);
 }
}

set_tptr()
{
 int i=0;
 file.open(masterfile,ios::in);
 if(!f_asso(file)) return 0;
 file.seekg(0,ios::end);
 tub=file.tellg()/sizeof(teacher);
 tptr=new teacher[tub];
 if(tptr==CHNULL)
 {
  outtextxy(100,100,"Out of primary memory..");
  getch();
  return 0;
 }
 file.seekg(0);
 while(file.read((char*)&tptr[i],sizeof(tptr[i])))
  i++;
 file.close();
 return 1;
}

void bubble_sort()
{
 teacher temp;
 for(int i=0;i<tub;i++)
  for(int j=0;j<(tub-i)-1;j++)
   if(tptr[j].nop[dow]>tptr[j+1].nop[dow])
   {
    temp=tptr[j];
    tptr[j]=tptr[j+1];
    tptr[j+1]=temp;
   }
}

void arr_substitute()
{
 int tleave,i;
 cleardevice();
 if(!set_tptr())
  return;
 file.open(subfile,ios::out | ios::binary);
 if(f_asso(file))
  if(!check("Previous day/todays substitutions will be lost Continue?"))
   return;
 file.close();
 cleardevice();
 outtextxy(150,240,"Enter the no. of teachers on leave");
 tleave=getnum(4,500,240);
 sub= new substitute[tleave];
 if(sub==CHNULL)
 {
  outtextxy(100,150,"Out of primary memory...");
  getch();
  return;
 }
 file.open(subfile,ios::out | ios::binary);
 if(!f_asso(file))
  return;
 for(i=0;i<tleave;i++)
  sub[i].accept();
 for(i=0;i<tleave;i++)
 {
  sub[i].arr_substitute();
  if(sub[i].isvalid())//if user entered a valid teacher name
   file.write((char*)&sub[i],sizeof(sub[i]));
 }
 file.close();
 cleardevice();
 outtextxy(150,200,"Check for substitutes-View Substitutes");
 getch();
 delete sub;
}

void view_substitute()
{
 cleardevice();
 file.open(subfile,ios::in | ios::binary);
 if(!f_asso(file))
  return;
 if(file.tellg()!=0)
 {
   outtextxy(100,100,"File is empty... Please arrange substitutes");
   getch();
 }
 else
  while(file.read((char*)&s,sizeof(s)))
   s.display();
 file.close();
}

void m_list()
{
 loading();
 options("Menu","Substitution ->",
	  "Database ->","How to use","About","Quit");
}

void sub_list()
{
 loading();
 options("Substitution","Arrange Substitutes",
	  "View Substitutes","Back <-");
}

void db_list()
{
 loading();
 options("Database","Add a record",
	  "View records","Modify a record",
	  "Delete a record","Back <-");
}

void fwrite()
{
 file.open(masterfile,ios::app | ios::binary);
 if(!f_asso(file)) return;
 if(t.accept())
  file.write((char*)&t,sizeof(t));
 file.close();
}

void fread()
{
 file.open(masterfile,ios::in | ios::binary);
 if(!f_asso(file)) return;
 file.seekg(0,ios::end);
 if(file.tellg()==0)
 {
  cleardevice();
  outtextxy(200,200,"No Records to display");
  getch();
 }
 else
 {
  file.seekg(0);
  while(file.read((char*)&t,sizeof(t)))
  t.display();
 }
 file.close();
}

void fmodify()
{
 int flag=0,var;
 outtextxy(50,50,"Enter record to be modified");
 var=getnum(4,50,70);
 getch();
 file.open(masterfile,ios::in | ios::out | ios::binary);
 if(!f_asso(file)) return;
 while(file.read((char*)&t,sizeof(t)))
 if(t.rtn_code()==var)
 {
  flag=1;
  file.seekg((long)-1*(long)sizeof(t),ios::cur);
  t.accept(var);
  file.write((char*)&t,sizeof(t));
  break;
 }
 cleardevice();
 if(!flag)
  outtextxy(200,200,"Record with this code not found");
 else
  outtextxy(200,200,"Modification successful!");
 getch();
 file.close();
}

void fdelete()
{
 int flag=0,var;
 outtextxy(50,50,"Enter record to be deleted");
 var=getnum(4,50,70);
 file.open(masterfile,ios::in | ios::binary);
 f.open(tempfile,ios::out|ios::binary);
 if(!f_asso(file) || !f_asso(f))
  return;
 while(file.read((char*)&t,sizeof(t)))
  if(t.rtn_code()!=var)
   f.write((char*)&t,sizeof(t));
  else
   flag=1;
 if(!flag)
 {
  outtextxy(200,150,"Record with this code not found");
  getch();
 }
 f.close();
 file.close();
 remove(masterfile);
 if (rename(tempfile, masterfile) != 0 || !flag)
  outtextxy(200,200,"Deletion unsuccessful!");
 else
  outtextxy(200,200,"Deletion successful!");
 getch();
}

int f_asso(fstream &f_obj)
{
 if(!f_obj)
 {
  cleardevice();
  settextstyle(SANS_SERIF_FONT,HORIZ_DIR,1);
  outtextxy(150,200,"Error!! file is in use or dosen't exist");
  getch();
  return 0;
 }
 return 1;
}

void set_date()
{
 date *dt=new date;
 getdate(dt);
 dd=(int)dt->da_day;
 mm=(int)dt->da_mon;
 yy=(int)dt->da_year;
 delete dt;
}

void set_dayofwk()
{
 dosdate_t *d=new dosdate_t;
 _dos_getdate(d);
 dow=(int)d->dayofweek;//0->Sunday 1->Monday
 --dow;//0->Monday
 if(dow>4)
 {
  gotoxy(20,10);
  cout<<"Today is a holiday...";
  getch();
  exit(0);
 }
 delete d;
}

getnum(int size,int x,int y)
{
 char str[5]="\0";
 int i=0;
 char temp[2];
 outtextxy(x-5,y,"|");
 do
 {
  str[i]=getch();
  temp[0]=str[i];
  temp[1]=CHNULL;
  str[i+1]=CHNULL;
  if(str[i]==SPACE || str[i]==ENTER)
  {
   str[i]=CHNULL;
   break;
  }
  else if(temp[0]==BACKSPACE && i>0)
  {
   i--;
   temp[0]=str[i];
   temp[1]=CHNULL;
   setcolor(BLUE);
   outtextxy(x+textwidth(str)-textwidth(temp),y,temp);
   setcolor(WHITE);
  }
  else if(isdigit(str[i]))
  {
   outtextxy(x+textwidth(str),y,temp);
   i++;
  }
 }
 while(i<size);
 setcolor(BLUE);
 outtextxy(x-5,y,"|");
 setcolor(WHITE);
 return atoi(str);
}

char *getstr(int size,int x,int y)
{
 char str[11]="\0";
 int i=0;
 char temp[2];
 outtextxy(x-5,y,"|");
 do
 {
  str[i]=getch();
  temp[0]=str[i];
  temp[1]=CHNULL;
  str[i+1]=CHNULL;
  if(str[i]==SPACE || str[i]==ENTER)
  {
   str[i]=CHNULL;
   break;
  }
  else if(temp[0]==BACKSPACE && i>0)
  {
   i--;
   temp[0]=str[i];
   temp[1]=CHNULL;
   setcolor(BLUE);
   outtextxy(x+textwidth(str)-textwidth(temp),y,temp);
   setcolor(WHITE);
  }
  else if(isalpha(str[i]))
  {
   outtextxy(x+textwidth(str),y,temp);
   i++;
  }
 }
 while(i<size);
 setcolor(BLUE);
 outtextxy(x-5,y,"|");
 setcolor(WHITE);
 return str;
}

char *getalnum(int size,int x,int y)
{
 char str[11]="\0";
 int i=0;
 char temp[2];
 outtextxy(x-5,y,"|");
 do
 {
  str[i]=getch();
  temp[0]=str[i];
  temp[1]=CHNULL;
  str[i+1]=CHNULL;
  if(str[i]==SPACE || str[i]==ENTER)
  {
   str[i]=CHNULL;
   break;
  }
  else if(temp[0]==BACKSPACE && i>0)
  {
   i--;
   temp[0]=str[i];
   temp[1]=CHNULL;
   setcolor(BLUE);
   outtextxy(x+textwidth(str)-textwidth(temp),y,temp);
   setcolor(WHITE);
  }
  else if(isalnum(str[i]))
  {
   outtextxy(x+textwidth(str),y,temp);
   i++;
  }
 }
 while(i<size);
 setcolor(BLUE);
 outtextxy(x-5,y,"|");
 setcolor(WHITE);
 return str;
}

void main()
{
 clrscr();
 set_date();
 set_dayofwk();
 init_graph();
 initializing();
 int a=1;
 while(a==1)
  a=menu();
 delete tptr;//deleting all the records from primary memory
 closegraph();
}