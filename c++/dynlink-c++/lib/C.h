
class C {
private:
	float value;
public:
	static int staticFunction(int x)
	{
		return x * 2;
	}

	C();
	virtual ~C();

	void setValue(float f);
	float getValue() const;
};
