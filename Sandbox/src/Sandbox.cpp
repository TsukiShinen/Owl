#include <Owl.h>

class Sandbox : public Owl::Application
{
public:
	Sandbox()
	{
		
	}

	~Sandbox()
	{
		
	}
};

Owl::Application* Owl::CreateApplication()
{
	return new Sandbox();
}