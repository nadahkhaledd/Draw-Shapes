#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>;
#include <math.h>
#include <vector>
#include <fstream>
#include<stdio.h>
#include <iostream>
#include <sstream>

using namespace std;
/*  Declare Windows procedure  */

/**
* Name: Hossam Mohamed  ID: 20170090
* Name: Omnia Fares     ID: 20180055
* Name: Nadah Khaled    ID: 20180309
* Name: Magda Osama     ID: 20180207
* Name: Hagar Ehab      ID: 20180323
**/
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
void AddMenu(HWND);
HMENU HMenu;

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;         /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);


    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_HAND);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Code::Blocks Template Windows App"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               544,                 /* The programs width */
               375,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}
union OutCode
{
    unsigned All:4;
    struct
    {
        unsigned left:1,top:1,right:1,bottom:1;
    };
};
OutCode GetOutCode(double x,double y,int xleft,int ytop,int xright,int ybottom)
{
    OutCode out;
    out.All=0;
    if(x<xleft)
        out.left=1;
    else if(x>xright)
        out.right=1;
    if(y<ytop)
        out.top=1;
    else if(y>ybottom)
        out.bottom=1;
    return out;
}
void VIntersect(double xs,double ys,double xe,double ye,int x,double *xi,double *yi)
{
    *xi=x;
    *yi=ys+(x-xs)*(ye-ys)/(xe-xs);
}
void HIntersect(double xs,double ys,double xe,double ye,int y,double *xi,double *yi)
{
    *yi=y;
    *xi=xs+(y-ys)*(xe-xs)/(ye-ys);
}
void CohenSuth(HDC hdc,int xs,int ys,int xe,int ye,int xleft,int ytop,int xright,int ybottom)
{
    double x1=xs,y1=ys,x2=xe,y2=ye;
    OutCode out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
    OutCode out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
    while( (out1.All || out2.All) && !(out1.All & out2.All))
    {
        double xi,yi;
        if(out1.All)
        {
            if(out1.left)
                VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out1.top)
                HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out1.right)
                VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else
                HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x1=xi;
            y1=yi;
            out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
        }
        else
        {
            if(out2.left)
                VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out2.top)
                HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out2.right)
                VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else
                HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x2=xi;
            y2=yi;
            out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
        }
    }
    if(!out1.All && !out2.All)
    {

        MoveToEx(hdc,round(x1),round(y1),NULL);
        LineTo(hdc,round(x2),round(y2));
    }
}

///----------------------------------------------------------------///
void parametricline(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color)
{
    double dx = x2 - x1;
    double dy = y2 - y1;
    for (double i = 0; i < 1; i += 0.001)
    {
        int x = x1 + (dx * i);
        int y = y1 + (dy * i);
        SetPixel(hdc, x, y, color);
    }

}


void lineMidPoint(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color)
{

    int dx = x2 - x1;
    int dy = y2 - y1;
    int x = x1;
    int y = y1;
    int d = dx - 2*dy;
    int change1 = 2*(dx-dy);
    int change2 = -2*dy;
    SetPixel(hdc,x,y,color);
    while(x < x2)
    {
        if(d<0)
        {
            d = d +change1;
            y++;
        }
        else
            d = d + change2;
        x++;
        SetPixel(hdc,x,y,color);
    }

}


void DDA(HDC hdc, int xa, int ya, int xb, int yb, COLORREF color)
{
    int dx = xb - xa, dy = yb - ya, steps, k;
    double xIncrement, yIncrement, x = xa, y = ya;
    if (abs(dx) > abs(dy))
        steps = abs(dx);
    else
        steps = abs(dy);
    xIncrement = dx / (double) steps;
    yIncrement = dy / (double) steps;
    SetPixel(hdc, ((int) (x + 0.5)), ((int) (y + 0.5)), RGB(250, 1, 1));
    for (k = 0; k < steps; k++)
    {
        x += xIncrement;
        y += yIncrement;
        SetPixel(hdc, ((int) (x + 0.5)), ((int) (y + 0.5)), color);
    }
}

///---------------------------------------------------------------------------///
void Draw_eight_points(HDC hdc, double xc, double yc, double x, double y, COLORREF color, int Opt)
{
    SetPixel(hdc, xc + x, yc + y, color);
    SetPixel(hdc, xc + x, yc - y, color);
    SetPixel(hdc, xc - x, yc + y, color);
    SetPixel(hdc, xc - x, yc - y, color);
    SetPixel(hdc, xc + y, yc + x, color);
    SetPixel(hdc, xc + y, yc - x, color);
    SetPixel(hdc, xc - y, yc + x, color);
    SetPixel(hdc, xc - y, yc - x, color);
    if(Opt ==1)
    {
        parametricline(hdc,xc,yc,xc+y,yc - x, color);  /// 1 1
        parametricline(hdc,xc,yc,xc+x,yc - y, color);  /// 2 1
    }
    else if(Opt == 2)
    {
        parametricline(hdc,xc,yc,xc-x,yc-y, color);  /// 1 2
        parametricline(hdc,xc,yc,xc-y,yc-x, color);  /// 2 2
    }
    else if(Opt == 3)
    {
        parametricline(hdc,xc,yc,xc-y,yc+x, color);  ///3 1
        parametricline(hdc,xc,yc,xc-x,yc + y, color);  /// 2 3
    }
    else if(Opt == 4)
    {
        parametricline(hdc,xc,yc,xc+x,yc + y, color);  // 1 4
        parametricline(hdc,xc,yc,xc+y,yc + x, color);
    }
}


void Polar(HDC hdc, double xc, double yc, double r, COLORREF color, int Opt)
{
    double x = r, y = 0, theta = 0, dtheta = 1.0 / r;
    Draw_eight_points(hdc, xc, yc, x, y, color, Opt);
    while (x > y)
    {
        theta += dtheta;
        x = r * cos(theta);
        y = r * sin(theta);
        Draw_eight_points(hdc, xc, yc, round(x), round(y), color, Opt);
    }
}


void Direct(HDC hdc, double xc, double yc, double r, COLORREF color, int Opt)
{
    double x = 0, y = r, r2 = r * r;
    Draw_eight_points(hdc, xc, yc, x, y, color, Opt);
    while (x < y)
    {
        x++;
        y = sqrt(r2 - (x * x));
        Draw_eight_points(hdc, xc, yc, round(x), round(y), color, Opt);
    }
}


void iterativePolar(HDC hdc, double xc, double yc, double r, COLORREF color, int Opt)
{
    double x = r, y = 0, dtheta = 1.0 / r, c = cos(dtheta), s = sin(dtheta);
    Draw_eight_points(hdc, xc, yc, x, y, color, Opt);
    while (x > y)
    {
        double x1 = (x * c) - (y * s);
        y = (x * s) + (y * c);
        x = x1;
        Draw_eight_points(hdc, xc, yc, round(x), round(y), color, Opt);
    }
}

void modifiedCircleMidpoint(HDC hdc, int xc, int yc, double r, COLORREF color, int Opt)
{
    int x=0, y=r, d=1-r,c1=3,c2=5-2*r;
    Draw_eight_points(hdc,xc,yc,x,y,color, Opt);
    while(x<y)
    {
        if(d<0)
        {
            d += c1;
            c2 += 2;
        }
        else
        {
            d += c2;
            c2 += 4;
            y--;
        }
        c1 += 2;
        x++;
        Draw_eight_points(hdc,xc,yc,round(x),round(y),color, Opt);
    }

}
void midPointCircle(HDC hdc, int xc, int yc, double r, COLORREF color, int Opt)
{
    int x=0, y=r, d=1-r;
    Draw_eight_points(hdc,xc,yc,x,y,color, Opt);
    while(x<y)
    {
        if(d<0)
            d += 2*x+3;
        else
        {
            d += 2*(x-y)+5;
            y--;
        }
        x++;
        Draw_eight_points(hdc,xc,yc,round(x),round(y),color, Opt);
    }
}

///-------------------------------------------------------------///
void PointClipping(HDC hdc, int x,int y,int xleft,int ytop,int xright,int ybottom,COLORREF color)
{
    if(x>=xleft && x<= xright && y>=ytop && y<=ybottom)

        SetPixel(hdc,x,y,color);
}


///-------------------------------------------------------------///
int Roundd(double num)
{
    return (int) num+0.5;
}

void Draw_Four_Points(HDC hdc, int xc, int yc, int x, int y,COLORREF color)
{
    SetPixel(hdc, xc+x, yc+y, color);
    SetPixel(hdc, xc-x, yc+y, color);
    SetPixel(hdc, xc-x, yc-y, color);
    SetPixel(hdc, xc+x, yc-y, color);
}

void DirectEllipse(HDC hdc, int xc, int yc, int a, int b, COLORREF color)
{
    int x=0,y=b;
    double a2 = a * a;
    double b2 = b * b;
    while(x<a)
    {
        x++;
        y = Roundd(sqrt((double)((b2*(a2-x*x))/a2)));
        Draw_Four_Points(hdc,xc,yc,x,y, color);
    }
    x=a,y=0;
    while(y<b)
    {
        y++;
        x = Roundd(sqrt((double)((a2*(b2-y*y))/b2)));
        Draw_Four_Points(hdc,xc,yc,x,y, color);
    }
}

void PolarEllipse(HDC hdc, int xc, int yc, int a,int b, COLORREF color)
{
    int x=a,y=0;
    double num = ( (3*(a+b)) +sqrt((double)(3*a+b)*(a+3*b)) );
    num = num/4;
    Draw_Four_Points(hdc,xc,yc,x,y,color);
    double theta = 0, Dtheta = 1.0/num;
    while(x>0)
    {
        theta += Dtheta;
        x =  round(a * cos(theta));
        y =  round(b * sin(theta));
        Draw_Four_Points(hdc,xc,yc,x,y,color);
    }
}

///-------------------------------------------------------------------///

fstream file;
struct shape
{
    int x1, x2, y1, y2, generalRadius, additionalRadius;
    int WinXleft, WinXright, WinYtop, WinYbottom;
    int isFilled;
    COLORREF Color;

    shape()
    {
        x1 = x2 = y1 = y2 = generalRadius = additionalRadius = 0;
        WinXleft = WinXright = WinYtop = WinYbottom = 0;
        isFilled = Color=0;
    }

};
vector<shape> lines, circles,ellipses, Cpoints, Clines;

void addLine(int x1, int y1, int x2, int y2, COLORREF color)
{
    shape line;
    line.x1 = x1;
    line.y1 = y1;
    line.x2 = x2;
    line.y2 = y2;
    line.Color = color;
    lines.push_back(line);
}

void saveLine()
{
    file.open("line.txt", std::ofstream::out | std::ofstream::trunc);
    if (file) {
        for(int i=0; i<lines.size(); i++)
        {
            file << lines[i].x1 << " " << lines[i].y1 << " " <<
             lines[i].x2 << " " << lines[i].y2 << " " << lines[i].Color << endl;
        }
		file.close();
	}
}

void addCircle(int xc, int yc, int R, int fill, COLORREF color)
{
    shape circle;
    circle.x1 = xc;
     circle.y1 = yc;
    circle.generalRadius = R;
    circle.Color = color;
     circle.isFilled = fill;
    circles.push_back(circle);
}

void saveCircle()
{
    file.open("circle.txt", std::ofstream::out | std::ofstream::trunc);
    if (file)
    {
        for(int i=0; i<circles.size(); i++)
        {
            file << circles[i].x1 << " " << circles[i].y1 << " " << circles[i].generalRadius << " " <<
             circles[i].isFilled << " " << circles[i].Color << endl;

        }
        file.close();
	}
}

void addEllipse(int xc, int yc, int r1, int r2, COLORREF color)
{
    shape ellipse;
    ellipse.x1 = xc;
    ellipse.y1 = yc;
    ellipse.generalRadius = r1;
    ellipse.additionalRadius = r2;
    ellipse.Color = color;
    ellipses.push_back(ellipse);
}

void saveEllipse()
{
    file.open("ellipse.txt", std::ofstream::out | std::ofstream::trunc);
    if(file)
    {
        for(int i=0; i<ellipses.size(); i++)
        {
            file << ellipses[i].x1 << " " << ellipses[i].y1 << " " <<
             ellipses[i].generalRadius << " " << ellipses[i].additionalRadius << " " << ellipses[i].Color << endl;

        }
        file.close();
    }
}

void addClipPoint(int x, int y, int xleft, int ytop, int xright, int ybottom, COLORREF color)
{
    shape clipPoint;
    clipPoint.x1 = x;
    clipPoint.y1 = y;
    clipPoint.WinXleft = xleft;
    clipPoint.WinYtop = ytop;
    clipPoint.WinXright = xright;
    clipPoint.WinYbottom = ybottom;
    clipPoint.Color = color;
    Cpoints.push_back(clipPoint);
}

void saveClippingPoint()
{
    file.open("clippoint.txt", std::ofstream::out | std::ofstream::trunc);
    if(file)
    {
        for(int i=0; i<Cpoints.size(); i++)
        {
            file <<  Cpoints[i].WinXleft << " " << Cpoints[i].WinYtop << " " << Cpoints[i].WinXright << " " << Cpoints[i].WinYbottom << " " <<
            Cpoints[i].x1 << " " << Cpoints[i].y1 << " " << Cpoints[i].Color << endl;
        }
        file.close();
    }
}

void addClipLine(int x1, int y1, int x2, int y2, int xleft, int ytop, int xright, int ybottom, COLORREF color)
{
    shape clipline;
    clipline.x1 = x1;
    clipline.y1 = y1;
    clipline.x2 = x2;
    clipline.y2 = y2;
    clipline.WinXleft = xleft;
    clipline.WinYtop = ytop;
    clipline.WinXright = xright;
    clipline.WinYbottom = ybottom;
    clipline.Color = color;
    Clines.push_back(clipline);
}

void saveClippingLine()
{
    file.open("clipline.txt", std::ofstream::out | std::ofstream::trunc);
    if(file)
    {
        for(int i=0; i<Clines.size(); i++)
        {
            file << Clines[i].WinXleft << " " << Clines[i].WinYtop << " " << Clines[i].WinXright << " " << Clines[i].WinYbottom << " " <<
            Clines[i].x1 << " " << Clines[i].y1 << " " << Clines[i].x2 << " " << Clines[i].y2 << " " << Clines[i].Color << endl;
        }
        file.close();
    }
}
///******************************loadFiles*******************************///
vector <int> split(string data)
{
    vector <int> v;
    string sub="";
    int intV;

    for(int i=0;i<data.length();i++)
    {
        if(data[i]==' ')
        {
            stringstream str(sub);
            str>>intV;
            v.push_back(intV);
            sub="";
        }
        else
            sub+=data[i];
    }
    stringstream strr(sub);
    strr>>intV;
    v.push_back(intV);
    return v;
}
void loadLine(HDC hdc)
{
    ifstream File("line.txt");
    string data;
    while (getline (File, data))
    {
        vector <int> lineData=split(data);
        addLine(lineData[0],lineData[1],lineData[2],lineData[3],lineData[4]);
        parametricline(hdc,lineData[0],lineData[1],lineData[2],lineData[3],lineData[4]);

    }

    File.close();
}

void loadCircle(HDC hdc)
{
    ifstream File("circle.txt");
    string data;
    while (getline (File, data))
    {
        vector <int> lineData=split(data);
        addCircle(lineData[0],lineData[1],lineData[2],lineData[3],lineData[4]);
        midPointCircle(hdc,lineData[0],lineData[1],lineData[2],lineData[4],lineData[3]);

    }

    File.close();
}

void loadEllipse(HDC hdc)
{
    ifstream File("ellipse.txt");
    string data;
    while (getline (File, data))
    {
        vector <int> lineData=split(data);
        addEllipse(lineData[0],lineData[1],lineData[2],lineData[3],lineData[4]);
        DirectEllipse(hdc,lineData[0],lineData[1],lineData[2],lineData[3],lineData[4]);

    }

    File.close();
}

void loadClipPoint(HDC hdc)
{
    ifstream File("clippoint.txt");
    string data;
    while (getline (File, data))
    {
        vector <int> lineData=split(data);
        addClipPoint(lineData[4],lineData[5],lineData[0],lineData[1],lineData[2],lineData[3],lineData[6]);
        Rectangle(hdc,lineData[0],lineData[1],lineData[2],lineData[3]);
        PointClipping(hdc,lineData[4],lineData[5],lineData[0],lineData[1],lineData[2],lineData[3],lineData[6]);

    }

    File.close();
}

void loadClipLine(HDC hdc)
{
    ifstream File("clipline.txt");
    string data;
    while (getline (File, data))
    {
        vector <int> lineData=split(data);
        addClipLine(lineData[4],lineData[5],lineData[6],lineData[7],lineData[0],lineData[1],lineData[2],lineData[3],lineData[8]);
        Rectangle(hdc,lineData[0],lineData[1],lineData[2],lineData[3]);
        CohenSuth(hdc,lineData[4],lineData[5],lineData[6],lineData[7],lineData[0],lineData[1],lineData[2],lineData[3]);

    }

    File.close();
}

///-------------------------------------------------------------------///
/*  This function is called by the Windows function DispatchMessage()  */
int x1,y1,x2,y2,x,y,m,n,s,t;
int r1,r2;
double r;
COLORREF ColorOfShape=RGB(0,0,0);
int TypeOfCircle=0, TypeOfLine=0, TypeOfClipping=0, TypeOfEllipse=0, TypeOfFilling=0;
int Num_of_Points=0;
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hwnd);

    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:
        AddMenu(hwnd);
        break;

    case WM_COMMAND:
        switch(wParam)
        {
        case 1:
            TypeOfLine=1;
            TypeOfCircle=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            TypeOfFilling=0;

            break;
        case 2:
            TypeOfLine=2;
            TypeOfCircle=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            TypeOfFilling=0;
            break;
        case 3:
            TypeOfLine=3;
            TypeOfCircle=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            TypeOfFilling=0;
            break;
        case 4:
            TypeOfCircle=1;
            TypeOfFilling=0;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            break;
        case 5:
            TypeOfCircle=1;
            TypeOfFilling=1;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            break;
        case 6:
            TypeOfCircle=1;
            TypeOfFilling=2;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            break;
        case 7:
            TypeOfCircle=1;
            TypeOfFilling=3;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            break;
        case 8:
            TypeOfCircle=1;
            TypeOfFilling=4;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            break;
        case 9:
            TypeOfCircle=2;
            TypeOfFilling=0;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            break;
        case 10:
            TypeOfCircle=2;
            TypeOfFilling=1;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            break;
        case 11:
            TypeOfCircle=2;
            TypeOfFilling=2;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            break;
        case 12:
            TypeOfCircle=2;
            TypeOfFilling=3;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            break;
        case 13:
            TypeOfCircle=2;
            TypeOfFilling=4;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            break;
        case 14:
            TypeOfCircle=3;
            TypeOfFilling=0;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            break;
        case 15:
            TypeOfCircle=3;
            TypeOfFilling=1;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            break;
        case 16:
            TypeOfCircle=3;
            TypeOfFilling=2;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            break;
        case 17:
            TypeOfCircle=3;
            TypeOfFilling=3;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            break;
        case 18:
            TypeOfCircle=3;
            TypeOfFilling=4;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            break;
        case 19:
            TypeOfCircle=4;
            TypeOfFilling=0;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            break;
        case 20:
            TypeOfCircle=4;
            TypeOfFilling=1;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            break;
        case 21:
            TypeOfCircle=4;
            TypeOfFilling=2;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            break;
        case 22:
            TypeOfCircle=4;
            TypeOfFilling=3;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            break;
        case 23:
            TypeOfCircle=4;
            TypeOfFilling=4;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            break;
        case 24:
            TypeOfCircle=5;
            TypeOfFilling=0;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            break;
        case 25:
            TypeOfCircle=5;
            TypeOfFilling=1;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            break;
        case 26:
            TypeOfCircle=5;
            TypeOfFilling=2;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            break;
        case 27:
            TypeOfCircle=5;
            TypeOfFilling=3;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            break;
        case 28:
            TypeOfCircle=5;
            TypeOfFilling=4;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfClipping=0;
            break;

        case 29:
            ColorOfShape=RGB(255,0,0);
            break;
        case 30:
            ColorOfShape=RGB(0,0,255);
            break;
        case 31:
            ColorOfShape=RGB(0,255,0);
            break;
        case 32:
            ColorOfShape=RGB(0,0,0);
            break;
        case 33:
            ColorOfShape=RGB(255,255,255);
            break;
        case 34:
            ColorOfShape=RGB(255,255,0);
            break;
        case 35:
            ColorOfShape=RGB(128,0,128);
            break;
        case 36:
            ColorOfShape=RGB(255,0,255);
            break;
        case 37:
            ColorOfShape=RGB(128,128,128);
            break;
        case 38:
            ColorOfShape=RGB(0,255,255);
            break;
        case 39:
            TypeOfCircle=0;
            TypeOfClipping=1;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfFilling=0;

            break;
        case 40:
            TypeOfCircle=0;
            TypeOfClipping=2;
            TypeOfLine=0;
            TypeOfEllipse=0;
            TypeOfFilling=0;
            break;
        case 41:
            cout<<"Files are saved."<<endl;
            saveLine();
            saveCircle();
            saveEllipse();
            saveClippingPoint();
            saveClippingLine();
            break;
        case 42:
            cout<<"Files are loaded."<<endl;
            loadLine(hdc);
            loadCircle(hdc);
            loadEllipse(hdc);
            loadClipPoint(hdc);
            loadClipLine(hdc);
            break;
        case 43:

            InvalidateRect(hwnd, NULL, TRUE);
            cout<<"Screen is Cleared "<<endl;
            lines.clear();
            circles.clear();
            ellipses.clear();
            Cpoints.clear();
            Clines.clear();
            break;
        case 44:
            TypeOfCircle=0;
            TypeOfClipping=0;
            TypeOfLine=0;
            TypeOfEllipse=1;
            break;
        case 45:

            TypeOfCircle=0;
            TypeOfClipping=0;
            TypeOfLine=0;
            TypeOfEllipse=2;
            break;

        }
        break;

    case WM_LBUTTONDBLCLK:
        if(Num_of_Points==0)
        {

            x1=LOWORD(lParam);
            y1=HIWORD(lParam);
            Num_of_Points++;
        }
        else if(Num_of_Points==1)
        {

            x2=LOWORD(lParam);
            y2=HIWORD(lParam);
            switch(TypeOfLine)
            {
            case 1:
                cout<<"You are using DDA Algorithm for drawing the line "<<endl;
                addLine(x1,y1,x2,y2,ColorOfShape);
                DDA(hdc, x1, y1, x2, y2, ColorOfShape);
                break;
            case 2:
                cout<<"You are using Midpoint Algorithm for drawing the line "<<endl;
                addLine(x1,y1,x2,y2,ColorOfShape);
                lineMidPoint(hdc, x1, y1, x2, y2, ColorOfShape);
                break;
            case 3:
                cout<<"You are using Parametric Algorithm for drawing the line "<<endl;
                addLine(x1,y1,x2,y2,ColorOfShape);
                parametricline(hdc, x1, y1, x2, y2, ColorOfShape);
                break;
            }
            switch(TypeOfCircle)
            {
            case 1:
                cout<<"You are using Direct Algorithm for drawing the Circle "<<endl;
                x=abs(x2-x1);
                y=abs(y2-y1);
                r=sqrt((x*x)+(y*y));
                addCircle(x1,y1,r,TypeOfFilling, ColorOfShape);
                Direct(hdc,x1,y1,r,ColorOfShape,TypeOfFilling);
                break;
            case 2:
                cout<<"You are using Polar Algorithm for drawing the Circle "<<endl;
                x=abs(x2-x1);
                y=abs(y2-y1);
                r=sqrt((x*x)+(y*y));
                addCircle(x1,y1,r,TypeOfFilling, ColorOfShape);
                Polar(hdc,x1,y1,r,ColorOfShape,TypeOfFilling);
                break;
            case 3:
                cout<<"You are using iterativePolar Algorithm for drawing the Circle "<<endl;
                x=abs(x2-x1);
                y=abs(y2-y1);
                r=sqrt((x*x)+(y*y));
                addCircle(x1,y1,r,TypeOfFilling, ColorOfShape);
                iterativePolar(hdc,x1,y1,r,ColorOfShape,TypeOfFilling);
                break;
            case 4:
                cout<<"You are using MidPoint Algorithm for drawing the Circle "<<endl;
                x=abs(x2-x1);
                y=abs(y2-y1);
                r=sqrt((x*x)+(y*y));
                addCircle(x1,y1,r,TypeOfFilling, ColorOfShape);
                midPointCircle(hdc,x1,y1,r,ColorOfShape,TypeOfFilling);
                break;
            case 5:
                cout<<"You are using ModifiedMidPoint Algorithm for drawing the Circle "<<endl;
                x=abs(x2-x1);
                y=abs(y2-y1);
                r=sqrt((x*x)+(y*y));
                addCircle(x1,y1,r,TypeOfFilling, ColorOfShape);
                modifiedCircleMidpoint(hdc,x1,y1,r,ColorOfShape,TypeOfFilling);
                break;
            }
            if(TypeOfClipping!=0 || TypeOfEllipse!=0)  ///take 3 points
                Num_of_Points++;
            else
                Num_of_Points=0;
        }
        else if(Num_of_Points==2)
        {

            m=LOWORD(lParam);
            n=HIWORD(lParam);
            if (TypeOfClipping==1)
            {
                cout<<"You are using Point Clipping With Rectangle Window"<<endl;
                Rectangle(hdc,x1,y1,x2,y2);
                addClipPoint(m,n,x1,y1,x2,y2,ColorOfShape);
                PointClipping(hdc,m,n,x1,y1,x2,y2,ColorOfShape);
                Num_of_Points=0;
            }
            else if(TypeOfEllipse==1)
            {
                cout<<"You are using Direct Algorithm for drawing the Ellips"<<endl;
                r1=sqrt(pow((x2-x1),2)+pow((y2-y1),2));
                r2=sqrt(pow((m-x1),2)+pow((n-y1),2));
                addEllipse(x1,y1,r1,r2,ColorOfShape);
                DirectEllipse(hdc,x1,y1,r1,r2,ColorOfShape);
                Num_of_Points=0;
            }
            else if(TypeOfEllipse==2)
            {
                cout<<"You are using Polar Algorithm for drawing the Ellips"<<endl;
                r1=sqrt(pow((x2-x1),2)+pow((y2-y1),2));
                r2=sqrt(pow((m-x1),2)+pow((n-y1),2));
                addEllipse(x1,y1,r1,r2,ColorOfShape);
                PolarEllipse(hdc,x1,y1,r1,r2,ColorOfShape);
                Num_of_Points=0;
            }
            else
            {
                Num_of_Points++;

            }
        }
        else if(Num_of_Points==3)
        {
            cout<<"You are using Line Clipping With Rectangle Window"<<endl;
            s=LOWORD(lParam);
            t=HIWORD(lParam);
            Rectangle(hdc,x1,y1,x2,y2);
            addClipLine(m,n,s,t,x1,y1,x2,y2,0);
            CohenSuth(hdc,m,n,s,t,x1,y1,x2,y2);
            Num_of_Points=0;
        }

        break;

    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}


void AddMenu(HWND hwnd)
{
    HMenu = CreateMenu();
    HMENU shapes = CreateMenu();
    HMENU lineMenu = CreateMenu();
    HMENU circleMenu = CreateMenu();
    HMENU colorMenu = CreateMenu();
    HMENU ClippingMenu = CreateMenu();
    HMENU ScreenOptions= CreateMenu();
    HMENU ellipseMenu= CreateMenu();
    HMENU FillingMenu= CreateMenu();
    HMENU DirectMenu= CreateMenu();
    HMENU PolarMenu= CreateMenu();
    HMENU MidpointMenu= CreateMenu();
    HMENU ModifiedMidMenu= CreateMenu();
    HMENU IterativeMenu= CreateMenu();

    AppendMenu(HMenu, MF_POPUP, (UINT_PTR) shapes, "Shape");
    AppendMenu(HMenu, MF_POPUP, (UINT_PTR) ScreenOptions, "ScreenOptions");
    AppendMenu(HMenu, MF_POPUP, (UINT_PTR) colorMenu, "Color");
    AppendMenu(HMenu, MF_POPUP, (UINT_PTR) ClippingMenu, "Clipping");
    AppendMenu(shapes, MF_POPUP, (UINT_PTR) lineMenu, "Line");
    AppendMenu(shapes, MF_POPUP, (UINT_PTR) circleMenu, "Circle");
    AppendMenu(shapes, MF_POPUP, (UINT_PTR) ellipseMenu, "Ellipse");


    AppendMenu(lineMenu, MF_STRING, 1, "DDA");
    AppendMenu(lineMenu, MF_STRING, 2, "Midpoint");
    AppendMenu(lineMenu, MF_STRING, 3, "Parametric");


    AppendMenu(circleMenu, MF_POPUP, (UINT_PTR) DirectMenu, "Direct");
    AppendMenu(circleMenu, MF_POPUP, (UINT_PTR) DirectMenu, "Polar");
    AppendMenu(circleMenu, MF_POPUP, (UINT_PTR) DirectMenu, "iterativePolar");
    AppendMenu(circleMenu, MF_POPUP, (UINT_PTR) DirectMenu, "midPointCircle");
    AppendMenu(circleMenu, MF_POPUP, (UINT_PTR) DirectMenu, "modifiedCircleMidpoint");

    AppendMenu(DirectMenu,MF_STRING, 4, "No Filling" );
    AppendMenu(DirectMenu,MF_STRING, 5, "Filling First Quarter" );
    AppendMenu(DirectMenu,MF_STRING, 6, "Filling Second Quarter" );
    AppendMenu(DirectMenu,MF_STRING, 7, "Filling Third Quarter" );
    AppendMenu(DirectMenu,MF_STRING, 8, "Filling Fourth Quarter" );

    AppendMenu(PolarMenu,MF_STRING, 9, "No Filling" );
    AppendMenu(PolarMenu,MF_STRING, 10, "Filling First Quarter" );
    AppendMenu(PolarMenu,MF_STRING, 11, "Filling Second Quarter" );
    AppendMenu(PolarMenu,MF_STRING, 12, "Filling Third Quarter" );
    AppendMenu(PolarMenu,MF_STRING, 13, "Filling Fourth Quarter" );

    AppendMenu(IterativeMenu,MF_STRING, 14, "No Filling" );
    AppendMenu(IterativeMenu,MF_STRING, 15, "Filling First Quarter" );
    AppendMenu(IterativeMenu,MF_STRING, 16, "Filling Second Quarter" );
    AppendMenu(IterativeMenu,MF_STRING, 17, "Filling Third Quarter" );
    AppendMenu(IterativeMenu,MF_STRING, 18, "Filling Fourth Quarter" );

    AppendMenu(MidpointMenu,MF_STRING, 19, "No Filling" );
    AppendMenu(MidpointMenu,MF_STRING, 20, "Filling First Quarter" );
    AppendMenu(MidpointMenu,MF_STRING, 21, "Filling Second Quarter" );
    AppendMenu(MidpointMenu,MF_STRING, 22, "Filling Third Quarter" );
    AppendMenu(MidpointMenu,MF_STRING, 23, "Filling Fourth Quarter" );

    AppendMenu(ModifiedMidMenu,MF_STRING, 24, "No Filling" );
    AppendMenu(ModifiedMidMenu,MF_STRING, 25, "Filling First Quarter" );
    AppendMenu(ModifiedMidMenu,MF_STRING, 26, "Filling Second Quarter" );
    AppendMenu(ModifiedMidMenu,MF_STRING, 27, "Filling Third Quarter" );
    AppendMenu(ModifiedMidMenu,MF_STRING, 28, "Filling Fourth Quarter" );


    AppendMenu(ellipseMenu, MF_STRING, 44, "Direct");
    AppendMenu(ellipseMenu, MF_STRING, 45, "Polar");



    AppendMenu(colorMenu, MF_STRING, 29,  "Red");
    AppendMenu(colorMenu, MF_STRING, 30, "Blue");
    AppendMenu(colorMenu, MF_STRING, 31, "Green");
    AppendMenu(colorMenu, MF_STRING, 32, "Black");
    AppendMenu(colorMenu, MF_STRING, 33, "White");
    AppendMenu(colorMenu, MF_STRING, 34, "Yellow");
    AppendMenu(colorMenu, MF_STRING, 35, "Purple");
    AppendMenu(colorMenu, MF_STRING, 36, "Magenta");
    AppendMenu(colorMenu, MF_STRING, 37, "Gray");
    AppendMenu(colorMenu, MF_STRING, 38, "Cyan");


    AppendMenu(ClippingMenu, MF_STRING, 39, "Point");
    AppendMenu(ClippingMenu, MF_STRING, 40, "Line" );

    AppendMenu(ScreenOptions,MF_STRING, 41, "Save");
    AppendMenu(ScreenOptions,MF_STRING, 43, "Clear");
    AppendMenu(ScreenOptions,MF_STRING, 42, "Load");


    SetMenu(hwnd, HMenu);



}
