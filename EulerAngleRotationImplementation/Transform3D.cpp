#include <math.h>
#include <stdio.h>
#include <windows.h>
#include "DrawUtil3D.h"
#include "Draw3D.h"
#include "Global3D.h"
#include "Transform3D.h"
#include "TransformGlobal.h"

#include "Cube.h"
#include "Cone.h"
#include "Cylinder.h"
#include "Sphere.h"

#include <iostream>
#include <fstream>
using namespace std;
double arr[720][6];
ofstream myfile;
bool flag=true;

void reDraw(HWND hwnd)
{
  InvalidateRect(hwnd, NULL, 1);
}

void setDrawMode(MODE mode, HWND hwnd)
{
  gShpData.drawMode = mode;
  showStatusOfDrawing();
  setupMenus(hwnd);
}

void calculateSize(HWND hwnd)
{
  RECT rect;

  GetClientRect(hwnd, &rect);

  gDrawData.windowSize.cx = rect.right;
  gDrawData.windowSize.cy = rect.bottom;

  gDrawData.origin.x = rect.right >> 1; // half
  gDrawData.origin.y = gShpData.statusHt + 
    (gDrawData.windowSize.cy - gShpData.statusHt) /2;

  if (gShpData.hwndStatus != NULL)
  {
    SetWindowPos(gShpData.hwndStatus, HWND_TOP, 0, 0, 
       gDrawData.windowSize.cx, gShpData.statusHt, 
       0);
  }
}

void setupAndDrawShape(HWND hwndParent, SHAPE shape)
{
  /*gets the relvant parameters thru' a dialog box*/
  eraseShape();
  resetDataForShape();

  if (gShpData.shape != shape)
  {
    gShpData.shape = shape;

    if (gShpData.pShapeData != NULL)
    {
      free(gShpData.pShapeData);
    }
    switch(shape)
    {
      case CUBE:
        gShpData.pShapeData = malloc(sizeof(CUBE_DATA));
        break;
      case CONE:
        gShpData.pShapeData = malloc(sizeof(CONE_DATA));
        break;
      case CYLINDER:
        gShpData.pShapeData = malloc(sizeof(CYLINDER_DATA)); 
        break;
      case SPHERE:
        gShpData.pShapeData = malloc(sizeof(SPHERE_DATA));
        break;
      default:
        break;
    }
  }
  setDrawMode(DRAW_MODE, hwndParent);

  ShowWindow(gShpData.hwndStatus, TRUE);

  DialogBox(NULL,"SHAPEDB",hwndParent,(DLGPROC)DlgShape);

  switch(shape)
  {
    case CUBE:
      setCubeData(
        ((CUBE_DATA *)gShpData.pShapeData)->cubeSide);
      break;
    case CONE:
      setConeData(
        ((CONE_DATA *)gShpData.pShapeData)->coneBaseRadius,
        ((CONE_DATA *)gShpData.pShapeData)->coneHeight);
      break; 
    case CYLINDER:
      setCylinderData(
        ((CYLINDER_DATA *)gShpData.pShapeData)->cylBaseRadius,
        ((CYLINDER_DATA *)gShpData.pShapeData)->cylHeight);
      break;
    case SPHERE:
      setSphereData(
        ((SPHERE_DATA *)gShpData.pShapeData)->sphereRadius);
      break;
    default:
      break;
  }
  drawShape();

  setDrawMode(DRAWN_MODE, hwndParent);
  reDraw(hwndParent);
}

void initializeShapeData(HWND hdlg)
{
  switch(gShpData.shape)
  {
    case CUBE:
    {
      CUBE_DATA *pCubeData = (CUBE_DATA *)gShpData.pShapeData;
      pCubeData->cubeSide = 150;
      ShowWindow(GetDlgItem(hdlg, ID_VARLBL0), SW_HIDE);
      ShowWindow(GetDlgItem(hdlg, ID_VARFLD0), SW_HIDE);
      SetDlgItemText(hdlg,ID_VARLBL1,
                     "Length of each side of the cube:");
      SetDlgItemInt(hdlg,ID_VARFLD1,pCubeData->cubeSide,1);
      SetWindowText(hdlg, "Input: Cube Data");
      break;
    }
    case CONE:
      {
      CONE_DATA *pConeData = (CONE_DATA *)gShpData.pShapeData;
      pConeData->coneBaseRadius = 75;
      pConeData->coneHeight = 150;
      ShowWindow(GetDlgItem(hdlg, ID_VARLBL0), SW_SHOW);
      ShowWindow(GetDlgItem(hdlg, ID_VARFLD0), SW_SHOW);
      SetDlgItemText(hdlg,ID_VARLBL1,"Height of the cone:");
      SetDlgItemInt(hdlg,ID_VARFLD1,pConeData->coneHeight,1);
      SetDlgItemText(hdlg,ID_VARLBL0,"Radius of Base:");
      SetDlgItemInt(hdlg,ID_VARFLD0,
                    pConeData->coneBaseRadius,1);
      SetWindowText(hdlg, "Input: Cone Data");
      break;
      }
    case CYLINDER:
    {
      CYLINDER_DATA *pCylinderData = 
        (CYLINDER_DATA *)gShpData.pShapeData;
      pCylinderData->cylBaseRadius = 75;
      pCylinderData->cylHeight = 150;
      ShowWindow(GetDlgItem(hdlg, ID_VARLBL0), SW_SHOW);
      ShowWindow(GetDlgItem(hdlg, ID_VARFLD0), SW_SHOW);
      SetDlgItemText(hdlg,ID_VARLBL1,"Height of the Cylinder:");
      SetDlgItemInt(hdlg,ID_VARFLD1,
                    pCylinderData->cylHeight,1);
      SetDlgItemText(hdlg,ID_VARLBL0,"Radius of Base:");
      SetDlgItemInt(hdlg,ID_VARFLD0,
                    pCylinderData->cylBaseRadius,1);
      SetWindowText(hdlg, "Input: Cylinder Data");
      break;
    }
    case SPHERE:
    {
      SPHERE_DATA *pSphereData = 
        (SPHERE_DATA *)gShpData.pShapeData;
      pSphereData->sphereRadius = 150;
      ShowWindow(GetDlgItem(hdlg, ID_VARLBL0), SW_HIDE);
      ShowWindow(GetDlgItem(hdlg, ID_VARFLD0), SW_HIDE);
      SetDlgItemText(hdlg,ID_VARLBL1,"Radius of the Sphere:");
      SetDlgItemInt(hdlg,ID_VARFLD1,
                    pSphereData->sphereRadius,1);
      SetWindowText(hdlg, "Input: Sphere Data");
      break;
    }
    default:
      break;
  }
}

void readShapeData(HWND hdlg)
{
  char szText[200];

  switch(gShpData.shape)
  {
    case CUBE:
    {
      CUBE_DATA *pCubeData = 
        (CUBE_DATA *)gShpData.pShapeData;
      pCubeData->cubeSide = 
        GetDlgItemInt(hdlg,ID_VARFLD1,NULL,1);
      sprintf(szText, "%s: side = %d",
         szShapeType[gShpData.shape], pCubeData->cubeSide);
      SetDlgItemText(gShpData.hwndStatus,ID_VARLBL1, szText);
        break;
    }
    case CONE:
    {
      CONE_DATA *pConeData = 
        (CONE_DATA *)gShpData.pShapeData;
      pConeData->coneHeight = 
        GetDlgItemInt(hdlg,ID_VARFLD1,NULL,1);
      pConeData->coneBaseRadius =  
        GetDlgItemInt(hdlg,ID_VARFLD0,NULL,1);
      sprintf(szText, "%s: Ht = %d, r = %d",
        szShapeType[gShpData.shape], 
        pConeData->coneHeight, pConeData->coneBaseRadius); 
      SetDlgItemText(gShpData.hwndStatus,ID_VARLBL1, szText);
      break;
    }
    case CYLINDER:
    {
      CYLINDER_DATA *pCylinderData = 
        (CYLINDER_DATA *) gShpData.pShapeData;
      pCylinderData->cylHeight = GetDlgItemInt(hdlg,ID_VARFLD1,NULL,1);
      pCylinderData->cylBaseRadius = 
        GetDlgItemInt(hdlg,ID_VARFLD0,NULL,1);
      sprintf(szText, "%s: Ht = %d, r = %d",
        szShapeType[gShpData.shape], 
        pCylinderData->cylHeight, pCylinderData->cylBaseRadius);
      SetDlgItemText(gShpData.hwndStatus,ID_VARLBL1, szText);
      break;
    }
    case SPHERE:
    {
      SPHERE_DATA *pSphereData = 
        (SPHERE_DATA *) gShpData.pShapeData;
      pSphereData->sphereRadius = 
        GetDlgItemInt(hdlg,ID_VARFLD1,NULL,1);
      sprintf(szText, "%s: r = %d",
        szShapeType[gShpData.shape], 
      pSphereData->sphereRadius);
      SetDlgItemText(gShpData.hwndStatus,ID_VARLBL1, szText);
      break;
    }
    default:
      break;
  }
}

bool isValidData()
{
  char szText[200];
  if ((gDrawData.r < MIN_R) || (gDrawData.r > MAX_R))
  {
    sprintf(szText, 
      "r (%5.1f) should be within %5.1f and %5.1f",
      gDrawData.r, MIN_R, MAX_R);
    SetDlgItemText(gShpData.hwndStatus,ID_VARLBL0, szText);
    return false;
  }
  if ((gDrawData.theta < MIN_THETA) || 
      (gDrawData.theta > MAX_THETA))
  {
    sprintf(szText, 
      "theta (%5.1f) should be within %5.1f and %5.1f",
      gDrawData.theta, MIN_THETA, MAX_THETA);
    SetDlgItemText(gShpData.hwndStatus,ID_VARLBL0, szText);
    return false;
  }
  if ((gDrawData.phi < MIN_PHI) || 
      (gDrawData.phi > MAX_PHI))
  {
    sprintf(szText, 
      "phi (%5.1f) should be within %5.1f and %5.1f",
      gDrawData.phi, MIN_PHI, MAX_PHI);
    SetDlgItemText(gShpData.hwndStatus,ID_VARLBL0, szText);
    return false;
  }
  if (gDrawData.screen_dist > gDrawData.r)
  {
    sprintf(szText, 
      "r (%5.1f) should be greater than d(%5.1f)",
      gDrawData.r, gDrawData.screen_dist);
    SetDlgItemText(gShpData.hwndStatus,ID_VARLBL0, szText);
    return false;
  }
  sprintf(szText, 
    "r = %5.1f, theta = %5.1f, phi = %5.1f, dist = %5.1f",  
    gDrawData.r, gDrawData.theta,  
    gDrawData.phi, gDrawData.screen_dist);
  SetDlgItemText(gShpData.hwndStatus,ID_VARLBL0, szText);
  return true;
}

LRESULT CALLBACK DlgShape(HWND hdlg,UINT mess,WPARAM more,LPARAM pos)
{
  char szText[200];
  switch(mess)
  {
    case WM_INITDIALOG:
      initializeShapeData(hdlg);
      SetDlgItemInt(hdlg,ID_R,(int)gDrawData.r,1);
      SetDlgItemInt(hdlg,ID_THETA,(int)gDrawData.theta,1);
      SetDlgItemInt(hdlg,ID_PHI,(int)gDrawData.phi,1);
      SetDlgItemInt(hdlg,ID_DIST,
                    (int)gDrawData.screen_dist,1);
      break;
    case WM_COMMAND:
      switch(more)
      { 
        case ID_DRAW:
          readShapeData(hdlg);
          gDrawData.r = 
            (int)GetDlgItemInt(hdlg,ID_R,NULL,1);
          gDrawData.theta = 
            (int)GetDlgItemInt(hdlg,ID_THETA,NULL,1);
          gDrawData.phi = 
            (int)GetDlgItemInt(hdlg,ID_PHI,NULL,1);
          gDrawData.screen_dist = 
            GetDlgItemInt(hdlg,ID_DIST,NULL,1);
  
          if (!isValidData())
            break;
          compute3DCoefficients(gDrawData.r, 
            gDrawData.theta, gDrawData.phi);

          sprintf(szText, 
            "r = %5.1f, theta = %5.1f, phi = %5.1f, dist = %5.1f",
            gDrawData.r, gDrawData.theta, gDrawData.phi, 
            gDrawData.screen_dist);
          SetDlgItemText(gShpData.hwndStatus,ID_VARLBL0, szText);
          EndDialog(hdlg,FALSE);
      }
      break;
  }
  return 0;
}

void transform(POINT3D &pt)
{
  switch(gShpData.drawMode)
  {
    case TRANSLATE_MODE:
      translate3D(pt, gShpData.tx, gShpData.ty, gShpData.tz);
      break;
    case ROTATE_MODE:	
      	rotate3D(pt, gShpData.rotType, gShpData.angOfRotation);
      break;
    case SCALE_MODE:
      scale3D(pt, gShpData.sx, gShpData.sy, gShpData.sz);
      break;
    default: 
      break;
  }
}

void transformAllSurfaces()
{
  int i, j;

	for (i=0; i<shape.nSurfaces; i++)
	  {
	    for (j = 0; j < shape.surface[i].nPoints; j++)
	    {
	      transform(shape.surface[i].vertex[j]);
	    }
	  }		
}

void translate3D(POINT3D & cartesianPt, double delx, 
                 double dely, double delz)
{
  cartesianPt.x += delx;
  cartesianPt.y += dely;
  cartesianPt.z += delz;
}

void temp(POINT3D & cartesianPt, ROTATE_TYPE rotType, double angle)
{
	double d=-(3.14159265359*angle)/180;
	myfile.open ("data_Z_Sphere.csv", ios::out | ios::app);
	double x1,x2,y1,y2,z1,z2;
	x1=x2=cartesianPt.x;
	y1=y2=cartesianPt.y;
	z1=z2=cartesianPt.z;
	for(int i=1;i<=720;i++)
	{
		x1 = (x1 * cos(d) - y1 * sin(d) + 0.5);
  		y1 = (x1 * sin(d) + y1 * cos(d) + 0.5);
  		z1 = (z1 * cos(d) - x1 * sin(d) + 0.5);
		x1 = (z1 * sin(d) + x1 * cos(d) + 0.5);
		if(i%1==0)
		{
			x2=x1;
			y2=y1;
			z2=z1;
			myfile << x1 << "," << x2 << "," << y1 << "," << y2 << "," << z1 << "," << z2<<"\n";
			continue;
		}
    	x2 = (x2 - y2 * d + 0.5);
	    y2 = (x2 * d + y2 + 0.5);
		z2 = (z2 - x2 * d + 0.5);
		x2 = (z2 * d - x2  + 0.5);	
		myfile << x1 << "," << x2 << "," << y1<<","<<y2 << "," << z1 << "," << z2<<"\n";
	}
}
void rotate3D(POINT3D & cartesianPt, ROTATE_TYPE rotType, 
              double angle)
{
	double d=-(3.14159265359*angle)/180;
	
	temp(cartesianPt, rotType, angle);
	return;
	
	double x1,x2,y1,y2,z1,z2;
	if(false)
	{
	  	myfile.open ("data_Z_Sphere.csv", ios::out | ios::app);
		for(int i=1;i<=720 and flag;i++)
		{
			switch(rotType)
			{
				case ROTATE_X:
			  		cartesianPt.y = y1 = 
			    		(cartesianPt.y * cos(d) - cartesianPt.z * sin(d) + 0.5);
			  		cartesianPt.z = z1 =
			    		(cartesianPt.y * sin(d) + cartesianPt.z * cos(d) + 0.5);
			    	y2=(cartesianPt.y - cartesianPt.z * d + 0.5);
			    	z2=(cartesianPt.y * d + cartesianPt.z + 0.5);
			  	break;
				case ROTATE_Y:
					cartesianPt.z = z1 = 
						(cartesianPt.z * cos(d) - cartesianPt.x * sin(d) + 0.5);
					cartesianPt.x = x1 =
						(cartesianPt.z * sin(d) + cartesianPt.x * cos(d) + 0.5);
					z2=(cartesianPt.z - cartesianPt.x * d + 0.5);
					x2=(cartesianPt.z * d - cartesianPt.x  + 0.5);
			  	break;
				case ROTATE_Z:
			  		cartesianPt.x = x1 =
			    		(cartesianPt.x * cos(d) - cartesianPt.y * sin(d) + 0.5);
			  		cartesianPt.y = y1 = 
			    		(cartesianPt.x * sin(d) + cartesianPt.y * cos(d) + 0.5);
			    	x2=(cartesianPt.x - cartesianPt.y * d + 0.5);
			    	y2=(cartesianPt.x * d + cartesianPt.y + 0.5);
			  	break;
			}
			arr[i-1][0]=x1;
			arr[i-1][1]=x2;
			arr[i-1][2]=y1;
			arr[i-1][3]=y2;
			arr[i-1][4]=z1;
			arr[i-1][5]=z2;
			myfile << arr[i-1][0] << "," << arr[i-1][1] << "," << arr[i-1][2]<<","<<arr[i-1][3] << "," << arr[i-1][4] << "," << arr[i-1][5]<<"\n";
		}
		myfile.close();
		flag=false;
	}
	else if(true)
	{
		switch(rotType)
		{
			case ROTATE_X:
		  		cartesianPt.y = y1 =
		    		(cartesianPt.y * cos(d) - cartesianPt.z * sin(d) + 0.5);
		  		cartesianPt.z = z1 =
		    		(cartesianPt.y * sin(d) + cartesianPt.z * cos(d) + 0.5);
		  	break;
			case ROTATE_Y:
				cartesianPt.z = z1 = 
					(cartesianPt.z * cos(d) - cartesianPt.x * sin(d) + 0.5);
				cartesianPt.x = x1 =
					(cartesianPt.z * sin(d) + cartesianPt.x * cos(d) + 0.5);
		  	break;
			case ROTATE_Z:
		  		cartesianPt.x = x1 =
		    		(cartesianPt.x * cos(d) - cartesianPt.y * sin(d) + 0.5);
		  		cartesianPt.y = y1 =
		    		(cartesianPt.x * sin(d) + cartesianPt.y * cos(d) + 0.5);
		  	break;
		}
	}
	
  
}

void scale3D(POINT3D & cartesianPt, double sx, double sy, 
             double sz)
{
  cartesianPt.x *= sx;
  cartesianPt.y *= sy;
  cartesianPt.z *= sz;
}

void drawOutline()
{
  int i, j;
  for (i = 0; i < shape.nSurfaces; i++)
  {
    // draw edge
    move3D(shape.surface[i].vertex[0]);
    for (j = 1; j < shape.surface[i].nPoints; j++)
    {
      line3D(shape.surface[i].vertex[j]);
    }
    line3D(shape.surface[i].vertex[0]);
  }
}
