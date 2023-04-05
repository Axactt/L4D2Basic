#include"drawLogic.h"

// Drawing lines here by Calling ID3DXLine interface
// and D3DXCreateLine function to get address of interface
void DrawLine( IDirect3DDevice9* pDevice, float x1, float y1, float x2, float y2, float width, bool antialias, D3DCOLOR color )
{
	ID3DXLine* pLine{}; // Interface implements line drawing using textured triangles
	

	D3DXCreateLine( pDevice, &pLine );
	D3DXVECTOR2 line[] = { D3DXVECTOR2( x1,y1 ),D3DXVECTOR2( x2,y2 ) };
    pLine->SetWidth( width );
	if (antialias)
		pLine->SetAntialias( 1 );
	//pLine->Begin(); // exception is being thrown here when game is crashing on unhook
	pLine->Draw( line, 2, color );
	//pLine->End();
	pLine->Release();
}


//drawing filled rectangle function to be called in hookedEndscene
void DrawFillRect( IDirect3DDevice9* pDevice, int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b )
{
	D3DCOLOR rectColor = D3DCOLOR_XRGB( r, g, b );	//No point in using alpha because clear & alpha dont work!
	D3DRECT BarRect = { x, y, x + w, y + h };

	pDevice->Clear( 1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, rectColor, 0, 0 );
}
// a Drawline function wrapper to take Vector2 objects as parameters
void DrawLine( IDirect3DDevice9* pDevice, Vector2 src, Vector2 dst, float width, bool antialias, D3DCOLOR color )
{

	DrawLine( pDevice, src.m_x, src.m_y, dst.m_x, dst.m_y, width, antialias, color );

}
//Draw line using draw prmitive method
void Line3D( IDirect3DDevice9* pDevice, float X, float Y, float Z, float X2, float Y2, float Z2, D3DCOLOR Color )
{
	struct stD3DVertex
	{
		float x, y, z, rhw;
		unsigned long color;
	};
	stD3DVertex objData[2];

	objData[0].x = X;
	objData[0].y = Y;
	objData[0].z = Z;
	objData[0].rhw = 0.0f;
	objData[0].color = Color;

	objData[1].x = X2;
	objData[1].y = Y2;
	objData[1].z = Z2;
	objData[1].rhw = 0.0f;
	objData[1].color = Color;

	LPDIRECT3DVERTEXBUFFER9 g_VertexBuffer = NULL;


    #define D3DFVF_VERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
	pDevice->CreateVertexBuffer( sizeof( objData ), 0, D3DFVF_VERTEX, D3DPOOL_DEFAULT, &g_VertexBuffer, NULL );

	void* ptr;

	g_VertexBuffer->Lock( 0, sizeof( objData ), (void**) &ptr, 0 );

	memcpy( ptr, objData, sizeof( objData ) );

	g_VertexBuffer->Unlock();

	//pDevice->BeginScene();


	pDevice->SetStreamSource( 0, g_VertexBuffer, 0, sizeof( stD3DVertex ) );

	pDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
	pDevice->SetFVF( D3DFVF_VERTEX );
	//pDevice->SetFVF(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1);
	pDevice->DrawPrimitive( D3DPT_LINELIST, 0, 1 );


	//pDevice->EndScene();
	//OrigPresent(pDevice, 0, 0, 0, 0);
}



void Line3D( IDirect3DDevice9* pDevice, Vector2 src, Vector2 dst, D3DCOLOR Color )
{

	Line3D( pDevice, src.m_x, src.m_y, 1.0f, dst.m_x, dst.m_y, 1.0f, Color );



}
// drawing 2d boxes around the entities 
void DrawEspBox2D( IDirect3DDevice9* pDevice, Vector2 top, Vector2 bottom, int thickness, bool antialias, D3DCOLOR color )
//void DrawEspBox2D( IDirect3DDevice9* pDevice, Vector2 top, Vector2 bottom,  D3DCOLOR color )
{
	int height = std::abs( top.m_y - bottom.m_y );

	Vector2 topLeft{};
	topLeft.m_x = top.m_x - height / 4;
	topLeft.m_y = top.m_y;
	Vector2 topRight{};
	topRight.m_x = top.m_x + height / 4;
	topRight.m_y = top.m_y;
	Vector2 bottomLeft{};
	bottomLeft.m_x = bottom.m_x - height / 4;
	bottomLeft.m_y = bottom.m_y;
	Vector2 bottomRight{};
	bottomRight.m_x = bottom.m_x + height / 4;
	bottomRight.m_y = bottom.m_y;
	
	DrawLine( pDevice, topLeft, topRight, thickness, false, color );
	DrawLine( pDevice, bottomLeft, bottomRight, thickness, false, color );
	DrawLine( pDevice, topLeft, bottomLeft, thickness, false, color );
	DrawLine( pDevice, topRight, bottomRight, thickness, false, color );  
	/*
	Line3D( pDevice, topLeft, topRight,  color );
	Line3D( pDevice, bottomLeft, bottomRight, color );
	Line3D( pDevice, topLeft, bottomLeft, color );
	Line3D( pDevice, topRight, bottomRight,  color );  */

}
