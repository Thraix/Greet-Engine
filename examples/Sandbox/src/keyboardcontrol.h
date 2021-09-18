#pragma once

#include <math/Maths.h>

namespace Greet {
	class KeyboardControl
	{
	public:
		
	private:
		Vec2f velocity;
		float speed;
		bool yPos;
		bool yNeg;
		bool xPos;
		bool xNeg;
		uint32_t yPosKey;
		uint32_t yNegKey;
		uint32_t xPosKey;
		uint32_t xNegKey;

	public:
		KeyboardControl(uint32_t xPosKey, uint32_t xNegKey, uint32_t yPosKey, uint32_t yNegKey, float speed)
			:xPosKey(xPosKey), xNegKey(xNegKey),yPosKey(yPosKey),yNegKey(yNegKey), speed(speed)
		{
			yPos = false;
			yNeg = false;
			xPos = false;
			xNeg = false;
		}

		Vec2f GetVelocity()
		{
			velocity = Vec2f(0, 0);
			velocity.x += xPos ? speed : 0;
			velocity.x -= xNeg ? speed : 0;
			velocity.y += yPos ? speed : 0;
			velocity.y -= yNeg ? speed : 0;
			if(velocity.LengthSQ()!=0)
				velocity = velocity.Normalize()*speed;
			return velocity;
		}

		void onInput(int key, bool pressed)
		{
			xPos = key == xPosKey ? pressed : xPos;
			xNeg = key == xNegKey ? pressed : xNeg;
			yPos = key == yPosKey ? pressed : yPos;
			yNeg = key == yNegKey ? pressed : yNeg;

		}
	};
}
