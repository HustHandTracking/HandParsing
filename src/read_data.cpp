#include "read_data.hpp"

#define VALUE_LENGTH 6
#define SAMPLE_LENGTH 8
#define DIM_LENGTH 6
#define CLASS_LENGTH 6

// loads the sample database from file (which is a dat text file)
int read_data_from_dat(const char* samplePath, Mat data, Mat classes, struct rtreeparams* params,int Type)
{
	ifstream fid(samplePath, ios::_Nocreate);
	char value[VALUE_LENGTH];
	int value_tmp = 0; 
	if (!fid)
	{
		return 0;
	}
	char number_of_sample[SAMPLE_LENGTH];
	char dim_of_sample[DIM_LENGTH];
	char number_of_classe[CLASS_LENGTH];
	fid >> number_of_sample;
	fid >> dim_of_sample;
	fid >> number_of_classe;
	for (int iSample = 0; iSample < params->NUMBER_OF_TRAINING_SAMPLES; iSample++)
	{
		for (int iDim = 0; iDim <params->ATTRIBUTES_PER_SAMPLE + 3; iDim++)
		{
			if (iDim == 0)//read the pixel`s coord x
			{
				fid >> value;
				/*if (Type == TEST)
				{ 
					value_tmp = atoi(value);
					coord.at<short>(iSample, 0) = static_cast<int>(value_tmp);
				}*/
		    }
			else if (iDim == 1)//read the pixel`s coord y
			{
				fid >> value;
				/*if (Type == TEST)
				{ 
					value_tmp = atoi(value);
					coord.at<short>(iSample, 1) = static_cast<int>(value_tmp);
				}*/
			}
			else if (iDim == 2)//read the pixel`s classification
			{
				fid >> value;
				value_tmp = atoi(value);
				classes.at<float>(iSample, 0) = static_cast<float>(value_tmp);
			}
			else
			{
				fid >> value;
				value_tmp = atoi(value);
				data.at<float>(iSample, iDim - 3) = static_cast<float>(value_tmp);
				//skip the break '\n'
				if (iDim == params->ATTRIBUTES_PER_SAMPLE - 1)
				{
					fid.seekg(2, ios::cur);
				}
			}
		}
	}
	return 1;
}

int read_data_from_dat(const char* samplePath, Mat data, Mat classes, Mat coord, struct rtreeparams* params,int Type)
{
	ifstream fid(samplePath, ios::_Nocreate);
	char value[VALUE_LENGTH];
	int value_tmp = 0; 
	if (!fid)
	{
		return 0;
	}
	char number_of_sample[SAMPLE_LENGTH];
	char dim_of_sample[DIM_LENGTH];
	char number_of_classe[CLASS_LENGTH];
	fid >> number_of_sample;
	fid >> dim_of_sample;
	fid >> number_of_classe;
	for (int iSample = 0; iSample < params->NUMBER_OF_TRAINING_SAMPLES; iSample++)
	{
		for (int iDim = 0; iDim <params->ATTRIBUTES_PER_SAMPLE + 3; iDim++)
		{
			if (iDim == 0)//read the pixel`s coord x, because in matlab it begins from 1
			{
				fid >> value;
				if (Type == TEST)
				{ 
					value_tmp = atoi(value);
					coord.at<short>(iSample, 0) = static_cast<int>(value_tmp - 1);
				}
		    }
			else if (iDim == 1)//read the pixel`s coord y
			{
				fid >> value;
				if (Type == TEST)
				{ 
					value_tmp = atoi(value);
					coord.at<short>(iSample, 1) = static_cast<int>(value_tmp - 1);
				}
			}
			else if (iDim == 2)//read the pixel`s classification
			{
				fid >> value;
				value_tmp = atoi(value);
				classes.at<float>(iSample, 0) = static_cast<float>(value_tmp);
			}
			else
			{
				fid >> value;
				value_tmp = atoi(value);
				data.at<float>(iSample, iDim - 3) = static_cast<float>(value_tmp);
				//skip the break '\n'
				if (iDim == params->ATTRIBUTES_PER_SAMPLE - 1)
				{
					fid.seekg(2, ios::cur);
				}
			}
		}
	}
	return 1;
}
int read_data_from_dat(const char* samplePath, struct rtreeparams* params, int Type)
{
	ifstream fid(samplePath, ios::_Nocreate);
	if (!fid)
	{
		return 0;
	}
	char number_of_sample[SAMPLE_LENGTH];
	char dim_of_sample[DIM_LENGTH];
	char number_of_classe[CLASS_LENGTH];
	//read the number of samples
	fid >> number_of_sample;
	if (Type == TRAIN)
	{ 
		params->NUMBER_OF_TRAINING_SAMPLES = atoi(number_of_sample);
	}
	else if (Type == TEST)
	{
		params->NUMBER_OF_TESTING_SAMPLES = atoi(number_of_sample);
	}
	//read the dim of each sample
	fid >> dim_of_sample;
	params->ATTRIBUTES_PER_SAMPLE = atoi(dim_of_sample);
	//read the number of classes
	fid >> number_of_classe;
	params->NUMBER_OF_CLASSES = atoi(number_of_classe);
	return 1;

}
