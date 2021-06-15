#pragma once

struct Step_Data;

class Step_Transformer
{
public:
	void Transform(std::vector<Step_Data>& data, float extensionLength);
};

