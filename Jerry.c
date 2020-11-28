#include "Jerry.h"

/* initialize a new new planet object */
Planet* createPlanet(char* planetName, double x, double y, double z)
{
	if (planetName == NULL) return NULL;

	Planet* planet = (Planet*) malloc(sizeof(Planet));
	if(planet == NULL) return NULL;

	planet->_planetNmae = (char*) malloc(strlen(planetName) + 1);
	if (planet->_planetNmae == NULL) return NULL;

	strcpy(planet->_planetNmae, planetName);
	planet->_x = x;
	planet->_y = y;
	planet->_z = z;

	return planet;
}

/* initialize a new new origin object */
Origin* createOrigin(char* dimension, Planet* planet)
{
	if (dimension == NULL || planet->_planetNmae == NULL) return NULL;

	Origin* origin = (Origin*) malloc(sizeof(Origin));
	if (origin == NULL) return NULL;

	origin->_dimension = (char*) malloc(strlen(dimension) + 1);
	if (origin->_dimension == NULL) return NULL;

	strcpy(origin->_dimension, dimension);
	origin->_planet = planet;

	return origin;
}

/* initialize a new physical characteristic object */
PhysicalCharacteristics* createPhysicalCharacteristic(char* physicalName, double physicalValue)
{
	if (physicalName == NULL) return NULL;

	PhysicalCharacteristics* pc = (PhysicalCharacteristics*) malloc(sizeof(PhysicalCharacteristics));
	if (pc == NULL) return NULL;

	pc->_physicalName = (char*) malloc(strlen(physicalName) + 1);
	if (pc->_physicalName == NULL) return NULL;

	strcpy(pc->_physicalName, physicalName);
	pc->_physicalValue = physicalValue;

	return pc;
}

/* initialize a new jerry object*/
Jerry* createJerry(char* id, int happinessLevel, Origin* origin)
{
	if (id == NULL) return NULL;

	Jerry* jerry = (Jerry*) malloc(sizeof(Jerry));
	if (jerry == NULL) return NULL;

	jerry->_jerryID = (char*) malloc(strlen(id) + 1);
	if (jerry->_jerryID == NULL) return NULL;

	strcpy(jerry->_jerryID, id);
	jerry->_happinessLevel = happinessLevel;
	jerry->_jerryOrigin = origin;
	jerry->_jerryPC = NULL;
	jerry->_jerryPcCounter = 0;

	return jerry;
}

/* check if jerry have specific physical characteristic
 * if exist, return true, if not exist, return false */
bool checkPhysicalCharacteristics(Jerry* jerry, char* physicalName)
{
	if (jerry == NULL || physicalName == NULL) return false;

	int i;
	for (i = 0; i < jerry->_jerryPcCounter; i++)
	{
		if(strcmp(jerry->_jerryPC[i]->_physicalName, physicalName) == 0)
			return true;
	}

	/* the physical characteristic not exists in this jerry */
	return false;
}

/* adding a new physical characteristic to specific jerry */
status addPhysicalCharacteristics(Jerry* jerry, PhysicalCharacteristics* physical)
{
	if (jerry == NULL || physical == NULL) return failure;

	/* the jerry's physical characteristic already exist */
	if (checkPhysicalCharacteristics(jerry, physical->_physicalName) == true) return failure;

	/* if the list is empty, allocate memory for it */
	if (jerry->_jerryPcCounter == 0)
	{
		jerry->_jerryPC = (PhysicalCharacteristics**) malloc(sizeof(PhysicalCharacteristics*));
		if (jerry->_jerryPC == NULL)
			return failure;

		jerry->_jerryPC[0] = physical;
		jerry->_jerryPcCounter++;

		return success;
	}
	 /* the list is not empty */
	else
	{
		jerry->_jerryPC = (PhysicalCharacteristics**) realloc(jerry->_jerryPC, ((jerry->_jerryPcCounter) + 1) * sizeof(PhysicalCharacteristics*));
		if (jerry->_jerryPC == NULL) return failure;

		jerry->_jerryPC[jerry->_jerryPcCounter] = physical;
		jerry->_jerryPcCounter++;

		return success;
	}
}

/* removing physical characteristic from specific jerry */
status removePhysicalCharacteristics(Jerry* jerry, char* physicalName)
{
	if (jerry == NULL || physicalName == NULL || jerry->_jerryPcCounter == 0)
		return failure;

	if (checkPhysicalCharacteristics(jerry, physicalName) == false)
		return failure;

	if (jerry->_jerryPcCounter == 1) //if only one Type remains, empty the list
	{
		free(jerry->_jerryPC);
		jerry->_jerryPC = NULL;
		jerry->_jerryPcCounter--;

		return success;
	}
	else
	{
		int counter = 0;
		int i = 0;

		for (i = 0; i < jerry->_jerryPcCounter; i++)
		{
			if(strcmp(((jerry->_jerryPC)[i])->_physicalName, physicalName) == 0)
				break;
			else
				counter++;
		}

		for (int i = counter; i < jerry->_jerryPcCounter - 1; i++)
			*(jerry->_jerryPC[i] )= *(jerry->_jerryPC[i+1]);

		free(jerry->_jerryPC[(jerry->_jerryPcCounter)-1]);
		jerry->_jerryPC[(jerry->_jerryPcCounter)-1] = NULL;
		jerry->_jerryPcCounter--;

		jerry->_jerryPC = (PhysicalCharacteristics**) realloc(jerry->_jerryPC, (jerry->_jerryPcCounter) * sizeof(PhysicalCharacteristics*));
		if (jerry->_jerryPC == NULL) return failure;

		return success;
	}
}

double roundTwoDigit(double number)
{
	return ((int)(number * 100.0 + 0.50001)) / 100.0;
}

/* print jerry object */
status printJerry(Jerry* jerry)
{
	if (jerry == NULL) return failure;

	printf("Jerry , ID - %s : \n", jerry->_jerryID);
	printf("Happiness level : %d \n", jerry->_happinessLevel);
	printf("Origin : %s \n", jerry->_jerryOrigin->_dimension);

	printPlanet(jerry->_jerryOrigin->_planet);

	if (jerry->_jerryPcCounter > 0)
	{
		printf("Jerry's physical Characteristics available : \n");
		printf("\t");
		int i;
		for (i = 0; i < jerry->_jerryPcCounter; i++)
		{
			double value = roundTwoDigit(jerry->_jerryPC[i]->_physicalValue);
			if (i != jerry->_jerryPcCounter - 1)
				printf("%s : %0.2f , ",jerry->_jerryPC[i]->_physicalName, value);
			else
				printf("%s : %0.2f \n",jerry->_jerryPC[i]->_physicalName, value);
		}
	}
	return success;
}

/* print planet object */
status printPlanet(Planet* planet)
{
	if (planet == NULL) return failure;

	double x = roundTwoDigit(planet->_x);
	double y = roundTwoDigit(planet->_y);
	double z = roundTwoDigit(planet->_z);
	printf("Planet : %s (%0.2f,%0.2f,%0.2f) \n", planet->_planetNmae, x, y, z);

	return success;
}

/* free planet object */
status destroyPlanet(Planet* planet)
{
	if (planet == NULL) return failure;

	free(planet->_planetNmae);
	planet->_planetNmae = NULL;
	free(planet);
	planet = NULL;

	return success;
}

/* free origin object */
status destroyOrigin(Origin* origin)
{
	if (origin == NULL) return failure;

	free(origin->_dimension);
	origin->_dimension = NULL;
	free(origin);
	origin = NULL;

	return success;
}

/* free physical characteristic object */
status destroyPhysicalCharacteristic(PhysicalCharacteristics* pc)
{
	if (pc == NULL) return failure;

	free(pc->_physicalName);
	pc->_physicalName = NULL;
	free(pc);
	pc = NULL;

	return success;
}

/* free jerry object */
status destroyJerry(Jerry* jerry)
{
	if (jerry == NULL) return failure;

	free(jerry->_jerryID);
	jerry->_jerryID = NULL;

	int i;
	for (i = 0; i < jerry->_jerryPcCounter; i++)
		if(destroyPhysicalCharacteristic(jerry->_jerryPC[i]) == failure) return failure;

	free(jerry->_jerryPC);
	jerry->_jerryPC = NULL;

	if (destroyOrigin(jerry->_jerryOrigin) == failure) return failure;

	free(jerry);
	jerry = NULL;

	return success;
}
