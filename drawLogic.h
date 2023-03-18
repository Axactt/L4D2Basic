#pragma once
#include "Includes.h"

//drawing filled rectangle function to be called in hookedEndscene
void DrawFillRect( IDirect3DDevice9* pDevice, int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b );

// Drawing lines here by Calling ID3DXLine interface
// and D3DXCreateLine function to get address of interface
void DrawLine( IDirect3DDevice9* pDevice, float x1, float y1, float x2, float y2, float width, bool antialias, D3DCOLOR color );

// a DrawLine wrapper function using Vector2 object instead of floats, overload resolution to select function
void DrawLine( IDirect3DDevice9* pDevice, Vector2 src, Vector2 dst, float width, bool antialias, D3DCOLOR color );

void DrawEspBox2D( IDirect3DDevice9* pDevice, Vector2 top, Vector2 bottom, int thickness, bool antialias, D3DCOLOR color );