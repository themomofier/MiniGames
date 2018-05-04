#include<iostream>
#include<cmath>
#include<SFML/Graphics.hpp>

using namespace std;
using namespace sf;

constexpr int windowWidth{800}, windowHeight{600};
constexpr int countBlocksX{11}, countBlocksY{4};
constexpr float paddleWidth{60.F}, paddleHeight{20.F}, paddleVelocity{6.F};
constexpr float ballRadius{10.F}, ballVelocity{8.F};
constexpr float blockWidth{60.F}, blockHeight{20.F};

struct Ball{
	//Ball variables
	CircleShape shape;
	Vector2f velocity{-ballVelocity, -ballVelocity};
	
	//Default constructor
	Ball(float mX, float mY){
		shape.setPosition(mX,mY);
		shape.setRadius(ballRadius);
		shape.setFillColor(Color::Red);
		shape.setOrigin(ballRadius, ballRadius);
	}

	//
	void update(){
		shape.move(velocity);

		if(left() < 0) velocity.x = ballVelocity;
		else if(right() > windowWidth) velocity.x = -ballVelocity;

		if(top() < 0) velocity.y = ballVelocity;
		else if(bottom() > windowHeight) velocity.y = -ballVelocity;
	}
	
	//
	float x()      { return shape.getPosition().x; }
	float y()      { return shape.getPosition().y; }
	float left()   { return x() - shape.getRadius(); }
	float right()  { return x() + shape.getRadius(); }
	float top()    { return y() - shape.getRadius(); }
	float bottom() { return y() + shape.getRadius(); }
};

struct Rectangle{
	//Rectangle variables
	RectangleShape shape;

	//
	float x()      { return shape.getPosition().x; }
	float y()      { return shape.getPosition().y; }
	float left()   { return x() - shape.getSize().x / 2.F; }
	float right()  { return x() + shape.getSize().x / 2.F; }
	float top()    { return y() - shape.getSize().y / 2.F; }
	float bottom() { return y() + shape.getSize().y / 2.F; }
};

struct Paddle : public Rectangle{
	//
	Vector2f velocity;
	//
	Paddle(float mX, float mY){
		shape.setPosition(mX, mY);
		shape.setSize({paddleWidth, paddleHeight});
		shape.setFillColor(Color::Red);
		shape.setOrigin(paddleWidth / 2.F, paddleHeight / 2.F);
	}
	//
	void update(){
		shape.move(velocity);
		if(Keyboard::isKeyPressed(Keyboard::Key::Left) && left() >0)
			velocity.x = -paddleVelocity;
		else if(Keyboard::isKeyPressed(Keyboard::Key::Right) && right() < windowWidth)
			velocity.x = paddleVelocity;
		else velocity.x = 0;
	}
};

struct Brick : public Rectangle{
	//
	bool destroyed = false;
	
	Brick(float mX, float mY){
		shape.setPosition(mX, mY);
		shape.setSize({blockWidth, blockHeight});
		shape.setFillColor(Color::Yellow);
		shape.setOrigin(blockWidth / 2.F, blockHeight / 2.F);
	}
};

//
template <class T1, class T2>
bool isIntersecting(T1& mA, T2& mB){
	return mA.right() >= mB.left() && mA.left() <= mB.right() && mA.bottom() >= mB.top() && mA.top() <= mB.bottom();
}

//
void testCollision(Paddle& mPaddle, Ball& mBall){
	if(!isIntersecting(mPaddle, mBall)) return;
		
	mBall.velocity.y = -ballVelocity;
	if(mBall.x() < mPaddle.x()) { mBall.velocity.x = -ballVelocity; }
	else 						{ mBall.velocity.x = ballVelocity;  }
}

//
void testCollision(Brick& mBrick, Ball& mBall){
	if(!isIntersecting(mBrick, mBall)) return;

	mBrick.destroyed = true;
		
	float overlapLeft{mBall.right() - mBrick.left()};
	float overlapRight{mBrick.right() - mBall.left()};
	float overlapTop{mBall.bottom() - mBrick.top()};
	float overlapBottom{mBrick.bottom() - mBall.top()};

	bool ballFromLeft(abs(overlapLeft) < abs(overlapRight));
	bool ballFromTop(abs(overlapTop) < abs(overlapBottom));
		
	float minOverlapX{ballFromLeft ? overlapLeft : overlapRight};
	float minOverlapY{ballFromTop ? overlapTop : overlapBottom};

	if(abs(minOverlapX) < abs(minOverlapY))
		mBall.velocity.x = ballFromLeft ? -ballVelocity : ballVelocity;
	else
		mBall.velocity.y = ballFromTop ? -ballVelocity : ballVelocity;
}

int main(){
	Ball ball{windowWidth / 2, windowHeight / 2};
	Paddle paddle{windowWidth / 2, windowHeight / - 50};
	vector<Brick> bricks;

	for(int iX{0}; iX < countBlocksX; ++iX)
		for(int iY{0}; iY < countBlocksY; ++iY)
			bricks.emplace_back((iX + 1)*(blockWidth + 3) + 22, (iY + 2)*(blockHeight + 3));
		RenderWindow window{{windowWidth, windowHeight}, "Arkanoid - 9"};
	
	window.setFramerateLimit(60);

	while(true){
		window.clear(Color::Black);

		if(Keyboard::isKeyPressed(Keyboard::Key::Escape)) break;

		ball.update();
		paddle.update();
		testCollision(paddle, ball);
		for(auto& brick : bricks) testCollision(brick, ball);
		bricks.erase(remove_if(begin(bricks), end(bricks),
					[](const Brick& mBrick){ return mBrick.destroyed; }), end(bricks));
		window.draw(ball.shape);
		window.draw(paddle.shape);
		for(auto& brick : bricks) window.draw(brick.shape);
		window.display();
	}
	return 0;
}
