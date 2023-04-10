#include"drawLogic.h"
#include"Player.h"
// Drawing lines here by Calling ID3DXLine interface
// and D3DXCreateLine function to get address of interface

ID3DXLine* g_pLine{}; //Crteating a global variable here and stop creating/releasing it every frame Prevented the LAG

ID3DXFont* g_pFont{}; // craete a global font variable here and not inside loop


void DrawLine( IDirect3DDevice9* pDevice, float x1, float y1, float x2, float y2, float width, bool antialias, D3DCOLOR color )
{
 // Interface implements line drawing using textured triangles
	if(!g_pLine) // Only create a g_pLine variable if none is there
	D3DXCreateLine( pDevice, &g_pLine );
	D3DXVECTOR2 line[] = { D3DXVECTOR2( x1,y1 ),D3DXVECTOR2( x2,y2 ) };
	g_pLine->SetWidth( width );
	if (antialias)
		g_pLine->SetAntialias( 1 );
	//pLine->Begin(); // exception is being thrown here when game is crashing on unhook
	g_pLine->Draw( line, 2, color );
	//pLine->End();
	//pLine->Release();
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
void DrawTextEsp ( IDirect3DDevice9* pDevice,const char* text, float x, float y, D3DCOLOR color )
{
	RECT rect{};
	if (!g_pFont) // Only use  D3DxCreateFontA function if g_PFont Interface pointer is null
	//D3DXCreateFontA version has to be used,D3DXCreateFont default to D3DXCreateFontW which requires LPCWSTR type 2nd last parameter
	D3DXCreateFontA( pDevice, 14, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &g_pFont );

	SetRect( &rect, x + 1, y + 1, x + 1, y + 1 );

	g_pFont->DrawTextA( NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB( 255, 0, 0, 0 ) );

	SetRect( &rect, x , y , x , y  );
	g_pFont->DrawTextA( NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, color );

}

//Drwaing 3d boxes around entities

void DrawEspBox3D( IDirect3DDevice9* pDevice, Vector3 entityTop, Vector3 entityOrigin, Vector3 viewAngle, int width, int thickness, D3DCOLOR color )
{

	float height3D = abs(entityTop.m_z- entityOrigin.m_z );
	Vector3 o1, o2, o3, o4, t1, t2, t3, t4;
	o1.m_z = o2.m_z = o3.m_z = o4.m_z = entityOrigin.m_z;

	o1.m_x = entityOrigin.m_x + (cosf( angleRad( viewAngle.m_y + 45 ) ) * width);
	o1.m_y = entityOrigin.m_y+ (sinf( angleRad( viewAngle.m_y + 45 ) ) * width);

	o2.m_x = entityOrigin.m_x + (cosf( angleRad( viewAngle.m_y + 135 ) ) * width);
	o2.m_y = entityOrigin.m_y + (sinf( angleRad( viewAngle.m_y + 135 ) ) * width);

	o3.m_x = entityOrigin.m_x + (cosf( angleRad( viewAngle.m_y + 225 ) ) * width);
	o3.m_y = entityOrigin.m_y + (sinf( angleRad( viewAngle.m_y + 225 ) ) * width);

	o4.m_x = entityOrigin.m_x + (cosf( angleRad( viewAngle.m_y + 315 ) ) * width);
	o4.m_y = entityOrigin.m_y + (sinf( angleRad( viewAngle.m_y + 315 ) ) * width);

	t1.m_x = o1.m_x;
	t1.m_y = o1.m_y;
	t1.m_z = o1.m_z + height3D;

	t2.m_x = o2.m_x;
	t2.m_y = o2.m_y;
	t2.m_z = o2.m_z + height3D;

	t3.m_x = o3.m_x;
	t3.m_y = o3.m_y;
	t3.m_z = o3.m_z + height3D;

	t4.m_x = o4.m_x;
	t4.m_y = o4.m_y;
	t4.m_z = o4.m_z + height3D;

	Vector2 o1two, o2two, o3two, o4two, t1two, t2two, t3two, t4two;

	if (W2S( o1, o1two ) && W2S( o2, o2two ) && W2S( o3, o3two ) && W2S( o4, o4two ) && W2S( t1, t1two ) && W2S( t2, t2two ) && W2S( t3, t3two ) && W2S( t4, t4two ))
	{
		//Draw lines for columns
		DrawLine( pDevice,t1two, o1two, thickness, false, color );
		DrawLine( pDevice, t2two, o2two, thickness, false, color );
		DrawLine( pDevice, t3two, o3two, thickness, false, color );
		DrawLine( pDevice, t4two, o4two, thickness, false, color );
		//DrawLines for Top bases
		DrawLine( pDevice, t1two, t2two, thickness, false, color );
		DrawLine( pDevice, t2two, t3two, thickness, false, color );
		DrawLine( pDevice, t3two, t4two, thickness, false, color );
		DrawLine( pDevice, t4two, t1two, thickness, false, color );
		//DrawLines for bottom bases
		DrawLine( pDevice, o1two, o2two, thickness, false, color );
		DrawLine( pDevice, o2two, o3two, thickness, false, color );
		DrawLine( pDevice, o3two, o4two, thickness, false, color );
		DrawLine( pDevice, o4two, o1two, thickness, false, color );


	}


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
