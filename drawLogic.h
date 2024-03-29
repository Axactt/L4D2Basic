#pragma once
#include "Includes.h"

//drawing filled rectangle function to be called in hookedEndscene

extern ID3DXLine* g_pLine; // declaration of global variable(inherent external linkage) to put it in scope of current translation unit.


extern ID3DXFont* g_pFont; // declaration of global font variable

void DrawFillRect( IDirect3DDevice9* pDevice, int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b );

// Drawing lines here by Calling ID3DXLine interface
// and D3DXCreateLine function to get address of interface
void DrawLine( IDirect3DDevice9* pDevice, float x1, float y1, float x2, float y2, float width, bool antialias, D3DCOLOR color );

// a DrawLine wrapper function using Vector2 object instead of floats, overload resolution to select function
void DrawLine( IDirect3DDevice9* pDevice, Vector2 src, Vector2 dst, float width, bool antialias, D3DCOLOR color );

void DrawEspBox2D( IDirect3DDevice9* pDevice, Vector2 top, Vector2 bottom, int thickness, bool antialias, D3DCOLOR color );

//void DrawEspBox2D( IDirect3DDevice9* pDevice, Vector2 top, Vector2 bottom,  D3DCOLOR color );


void Line3D( IDirect3DDevice9* pDevice, float X, float Y, float Z, float X2, float Y2, float Z2, D3DCOLOR Color );

void Line3D( IDirect3DDevice9* pDevice, Vector2 src, Vector2 dst, D3DCOLOR Color );

void DrawEspBox3D( IDirect3DDevice9* pDevice,Vector3 entityTop,Vector3 entityOrigin, Vector3 viewAngle, int width, int thickness, D3DCOLOR color );

void DrawTextEsp( IDirect3DDevice9* pDevice, const char* text, float x, float y, D3DCOLOR color ); // for drawing text on screen