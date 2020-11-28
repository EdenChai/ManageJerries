#include "Jerry.h"
#define MAX_SIZE 300

char* uppercaseFunc(char s[]);
Planet* searchPlanet(char* planet_name, int numOfPlanets, Planet** planetArray);
Jerry* searchID(char* id, int numOfJerries, Jerry** jerriesArray);
bool searchPhysicalCharacteristic(char* pc_name, int numOfJerries, Jerry** jerriesArray);
status deleteAllPlanets(Planet** planetArray, int numOfPlanets);
status deleteAllJerries(Jerry** jerriesArray, int numOfJerries);


int main(int argc, char* argv[])
{
	int numOfPlanets = atoi(argv[1]);	// load Planet amount
	int numOfJerries = atoi(argv[2]);	// load Jerries amount
	int planetsArrSize = 0;
	int jerryArrSize = 0;

	Planet** planetsArray = (Planet**) malloc(numOfPlanets*sizeof(Planet*)); // array of planets
	if(planetsArray == NULL)
	{
		printf("Memory Problem\n");
		deleteAllPlanets(planetsArray, numOfPlanets);
		exit(1);
	}

	Jerry** jerriesArray = (Jerry**) malloc(numOfJerries*sizeof(Jerry*)); // array of Jerries
	if(jerriesArray == NULL)
	{
		printf("Memory Problem\n");
		deleteAllPlanets(planetsArray, numOfPlanets);
		deleteAllJerries(jerriesArray, numOfJerries);
		exit(1);
	}

	/* start to reading the file configuration */

	FILE* f;
	char str[MAX_SIZE];
	f = fopen(argv[3], "r");
	fgets(str, MAX_SIZE, f);

	while (strstr(fgets(str, MAX_SIZE, f), "Jerries") == NULL ) // insert all planets to the planetsArray
	{
			char* planet_name;
			double x, y, z;
			char* token = str;

			token = strtok(token, ",");
			planet_name = token;

			token = strtok(NULL, ",");
			x = atof(token);

			token = strtok(NULL, ",");
			y = atof(token);

			token = strtok(NULL,"\n\r");
			z = atof(token);

			planetsArray[planetsArrSize] = createPlanet(planet_name, x, y, z);
			if (planetsArray[planetsArrSize] == NULL)
			{
				printf("Memory Problem\n");
				deleteAllPlanets(planetsArray, numOfPlanets);
				deleteAllJerries(jerriesArray, numOfJerries);
				exit(1);
			}
			planetsArrSize++;
	}

	while (fgets(str, MAX_SIZE, f) != NULL)  // insert all jerries to the jerriesArray
	{
		if (strstr(str, "\t") == NULL)
		{
			char* id;
			char* dimension;
			char* planet_name;
			int happiness_level;
			char* token = str;

			token = strtok(token, ",");
			id = token;

			token = strtok(NULL, ",");
			dimension = token;

			token = strtok(NULL, ",");
			planet_name = token;

			token = strtok(NULL,"\n\r");
			happiness_level = atoi(token);

			/* searching if the plant already exist in planetsArray */
			Planet* planet = searchPlanet(planet_name, numOfPlanets, planetsArray);

			/* create new origin for jerry */
			Origin* origin =  createOrigin(dimension, planet);
			if (origin == NULL)
			{
				printf("Memory Problem\n");
				deleteAllPlanets(planetsArray, numOfPlanets);
				deleteAllJerries(jerriesArray, numOfJerries);
				exit(1);
			}

			/* create new jerry and adding him to jerriesArray */
			jerriesArray[jerryArrSize] = createJerry(id, happiness_level, origin);
			if (jerriesArray[jerryArrSize] == NULL)
			{
				printf("Memory Problem\n");
				deleteAllPlanets(planetsArray, numOfPlanets);
				deleteAllJerries(jerriesArray, numOfJerries);
				exit(1);
			}
			jerryArrSize++;
		}
		else // adding physical characteristic to jerry
		{
			char* cp_name;
			double value;
			char* token = str;

			token = strtok(token, "\t");

			token = strtok(token, ":");
			cp_name = token;

			token = strtok(NULL, "\n\r");
			value = atof(token);

			PhysicalCharacteristics* pc = createPhysicalCharacteristic(cp_name, value);
			if (pc == NULL)
			{
				printf("Memory Problem\n");
				deleteAllPlanets(planetsArray, numOfPlanets);
				deleteAllJerries(jerriesArray, numOfJerries);
				exit(1);
			}

			addPhysicalCharacteristics(jerriesArray[jerryArrSize - 1], pc);

		}
	}
	fclose(f);

	/* finish to reading the file configuration */

		char input[MAX_SIZE];
		char choice;

		do
		{
			printf("AW JEEZ RICK, what do you want to do now ? \n");
			printf("1 : Print all Jerries \n");
			printf("2 : Print all Planets \n");
			printf("3 : Add physical characteristic to Jerry \n");
			printf("4 : Remove physical characteristic from Jerry \n");
			printf("5 : Print Jerries by a planet \n");
			printf("6 : Print Jerries by a physical characteristic \n");
			printf("7 : Go home \n");

			scanf("%s",input);
			if (strlen(input) != 1) // check if the input is one char
				choice = '9';
			else
				choice = input[0];

			switch (choice)
			{

			case '1':  // Print all Jerries

				for (int i = 0; i < numOfJerries; i++)
					printJerry(jerriesArray[i]);
				break;


			case '2':  // Print all Planets

				for (int i = 0; i < numOfPlanets; i++)
					printPlanet(planetsArray[i]);
				break;


			case '3':  // Add physical characteristic to Jerry
				printf("What is your Jerry's ID ? \n");
				char id1[MAX_SIZE];
				scanf("%s",id1);
				Jerry* jerry1 = searchID(id1,numOfJerries,jerriesArray);

				/* the jerry ID doesn't exist */
				if(jerry1 == NULL)
				{
					printf("OH NO! I CAN'T FIND HIM RICK ! \n");
					break;
				}

				printf("What physical characteristic can you add to Jerry - %s ? \n", id1);
				char pc_name1[MAX_SIZE];
				scanf("%s", pc_name1);

				/* the physical characteristic already exist */
				if(checkPhysicalCharacteristics(jerry1, pc_name1) == true)
				{
					char* pcName1 = uppercaseFunc(pc_name1);
					printf("RICK I ALREADY KNOW HIS %s ! \n", pcName1);
					break;
				}

				printf("What is the value of his %s ? \n", pc_name1);
				char char_value[MAX_SIZE];
				double double_value;
				scanf("%s", char_value);
				double_value = atof(char_value);

				PhysicalCharacteristics* pc = createPhysicalCharacteristic(pc_name1, double_value);
				if (pc == NULL)
				{
					printf("Memory Problem\n");
					deleteAllPlanets(planetsArray, numOfPlanets);
					deleteAllJerries(jerriesArray, numOfJerries);
					exit(1);
				}

				addPhysicalCharacteristics(jerry1, pc);
				printJerry(jerry1);
				break;


			case '4':	// Remove physical characteristic from Jerry
				printf("What is your Jerry's ID ? \n");
				char id2[MAX_SIZE];
				scanf("%s",id2);
				Jerry* jerry2 = searchID(id2,numOfJerries,jerriesArray);

				/* the jerry ID doesn't exist */
				if(jerry2 == NULL)
				{
					printf("OH NO! I CAN'T FIND HIM RICK ! \n");
					break;
				}

				printf("What physical characteristic do you want to remove from Jerry - %s ? \n", id2);
				char pc_name2[MAX_SIZE];
				scanf("%s", pc_name2);

				/* the physical characteristic doesn't exist */
				if(checkPhysicalCharacteristics(jerry2, pc_name2) == false)
				{
					char* pcName2 = uppercaseFunc(pc_name2);
					printf("RICK I DON'T KNOW HIS %s ! \n", pcName2);
					break;
				}

				removePhysicalCharacteristics(jerry2, pc_name2);
				printJerry(jerry2);
				break;


			case '5':	// Print Jerries by a planet

				printf("What planet is your Jerry from ? \n");
				char planet_name[MAX_SIZE];
				scanf("%s", planet_name);

				/* the planet doesn't exist */
				if(searchPlanet(planet_name, numOfPlanets, planetsArray) == NULL)
				{
					char* planetName = uppercaseFunc(planet_name);
					printf("RICK I NEVER HEARD ABOUT %s ! \n", planetName);
					break;
				}
				else /* the planet is exist in planet array */
				{
					int i, flag = 0;
					for(i = 0; i < numOfJerries; i++)
					{
						if(strcmp(jerriesArray[i]->_jerryOrigin->_planet->_planetNmae , planet_name) == 0)
						{
							flag = 1;
							printJerry(jerriesArray[i]);
						}
					}
					if(flag == 0)
					{
						char* planetName1 = uppercaseFunc(planet_name);
						printf("OH NO! I DON'T KNOW ANY JERRIES FROM %s ! \n", planetName1);
					}
				}
				break;

			case '6':	// Print Jerries by a physical characteristic
				printf("What do you know about your Jerry ? \n");
				char pc_name3[MAX_SIZE];
				scanf("%s", pc_name3);

				/* the physical characteristic doesn't exist in any jerry */
				if(searchPhysicalCharacteristic(pc_name3, numOfJerries, jerriesArray) == false)
				{
					char* pcName3 = uppercaseFunc(pc_name3);
					printf("OH NO! I DON'T KNOW ANY JERRY'S %s ! \n", pcName3);
					break;
				}

				else
				{
					int i;
					for (i = 0; i < numOfJerries; i++)
					{
						if (checkPhysicalCharacteristics(jerriesArray[i], pc_name3) == true)
							printJerry(jerriesArray[i]);
					}
				}
				break;

			case '7': // Go home

				deleteAllPlanets(planetsArray, numOfPlanets);
				deleteAllJerries(jerriesArray, numOfJerries);
				printf("AW JEEZ RICK, ALL THE JERRIES GOT FREE ! \n");
				break;

			default: //upon invalid input, exit
				printf("RICK WE DON'T HAVE TIME FOR YOUR GAMES ! \n");
				break;
			}
		} while(choice != '7');

	return 0;
}

/* function that change lowercase string to uppercase */
char* uppercaseFunc(char s[])
{
	int i;
	for (i = 0; s[i] != '\0'; i++)
	{
		if (s[i] >= 'a' && s[i] <= 'z')
			s[i] = s[i] -32;
	}
	return s;
}

/* searching specific planet in planetArray
 * if exist return pointer to planet
 * if not exist return NULL */
Planet* searchPlanet(char* planet_name, int numOfPlanets, Planet** planetArray)
{
	int i;
	for (i = 0; i < numOfPlanets; i++)
	{
		if (strcmp(planet_name, planetArray[i]->_planetNmae) == 0)
			return planetArray[i];
	}
	return NULL;
}

/* searching specific jerry in jerriesArray
 * if exist return pointer to jerry
 * if not exist return NULL */
Jerry* searchID(char* id, int numOfJerries, Jerry** jerriesArray)
{
	int i;
	for (i = 0; i < numOfJerries; i++)
	{
		if (strcmp(id, jerriesArray[i]->_jerryID) == 0)
			return jerriesArray[i];
	}
	return NULL;
}

/* searching specific physical characteristic in all jerries
 * if exist return true
 * if not exist return false */
bool searchPhysicalCharacteristic(char* pc_name, int numOfJerries, Jerry** jerriesArray)
{
	int i;
	for (i = 0; i < numOfJerries; i++)
	{
		if (checkPhysicalCharacteristics(jerriesArray[i], pc_name) == true)
			return true;
	}
	return false;
}

status deleteAllPlanets(Planet** planetsArray, int numOfPlanets)
{
	if (planetsArray == NULL) return failure;

	int i;
	for (i = 0; i < numOfPlanets; i++)
		destroyPlanet(planetsArray[i]);
	free(planetsArray);
	planetsArray = NULL;

	return success;
}

status deleteAllJerries(Jerry** jerriesArray, int numOfJerries)
{
	if (jerriesArray == NULL) return failure;

	int i;
	for(i = 0; i < numOfJerries; i++)
		destroyJerry(jerriesArray[i]);
	free(jerriesArray);
	jerriesArray = NULL;

	return success;
}
