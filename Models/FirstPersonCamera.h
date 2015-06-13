#ifndef _FP_CAM_H_
#define _FP_CAM_H_

#include<xnamath.h>


class FirstPersonCamera
{
	public:
		FirstPersonCamera(void);
		~FirstPersonCamera(void);


		void SetDistance( float distance, float minDistance, float maxDistance );
		void SetRotation( float x, float y, float minY, float maxY );
		//ALEX OWEN 16/04
		XMVECTOR GetRotationVec();
		void SetTarget( XMFLOAT3& target );
		void SetPosition(float x, float y, float z);
		XMFLOAT3 GetPosition( );
		void SetDirection(float x, float y, float z);
		void Move(float leftRight, float backFoward);

		void ApplyZoom( float zoomDelta );
		void ApplyRotation( float yawDelta, float pitchDelta );

		float xRotation_, yRotation_, yMin_, yMax_;

		XMMATRIX GetViewMatrix( );

		XMFLOAT3 position_;

	private:
		
		XMFLOAT3 target_;
		XMFLOAT3 direction_;

		float distance_, minDistance_, maxDistance_;
		

		XMVECTOR DefaultForward_ ;
		XMVECTOR DefaultRight_;
		XMVECTOR camForward_;
		XMVECTOR camRight_;

		XMMATRIX camView_;

		XMVECTOR camUp_;
};

#endif