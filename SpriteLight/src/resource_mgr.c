#define INITIAL_TABLE_SIZE 16
#define LOAD_FACTOR_THRESHOLD 0.75

void InitHashTable(size_t initial_capacity)
{
	state->salad = (Salad *)malloc(sizeof(Salad));
	Salad *salad = state->salad;
	if (!salad)
	{
		HandleError("Error: Failed to allocate memory for Salad\n");
		return;
	}

	salad->hash_table = (HashTable *)malloc(sizeof(HashTable));
	if (!salad->hash_table)
	{
		HandleError("Error: Failed to allocate memory for the hash table\n");
		return;
	}

	salad->hash_table->size = 0;
	salad->hash_table->capacity = initial_capacity;
	salad->hash_table->table = (HashNode **)calloc(initial_capacity, sizeof(HashNode *));
	if (!salad->hash_table->table)
	{
		HandleError("Error: Failed to allocate memory for the hash table entries\n");
		return;
	}

	salad->next_index = 0;
}

// Calculate a hash value for a given string
unsigned int Hash(const char *str)
{
	unsigned int hash = 0;
	while (*str)
	{
		hash = (hash * 31) + *str;
		str++;
	}
	return hash;
}

// Error handling function
void HandleError(const char *message, ...)
{
	va_list args;
	va_start(args, message);
	vfprintf(stderr, message, args);
	va_end(args);
	exit(1);
}
// Resize the hash table
void ResizeHashTable()
{
	Salad *salad = state->salad;
	size_t new_capacity = salad->hash_table->capacity * 2;
	HashNode **new_table = (HashNode **)calloc(new_capacity, sizeof(HashNode *));
	if (!new_table)
	{
		HandleError("Failed to allocate memory for the new hash table");
		return;
	}

	for (size_t i = 0; i < salad->hash_table->capacity; i++)
	{
		HashNode *current = salad->hash_table->table[i];
		while (current)
		{
			HashNode *next = current->next;
			unsigned int new_index = Hash(current->resource.file_path) % new_capacity;
			current->next = new_table[new_index];
			new_table[new_index] = current;
			current = next;
		}
	}

	free(salad->hash_table->table);
	salad->hash_table->table = new_table;
	salad->hash_table->capacity = new_capacity;
}

// Function to load a resource
Resource *LoadResource(const char *file_path)
{
	Salad *salad = state->salad;
	unsigned int file_hash = Hash(file_path);
	size_t index = file_hash % salad->hash_table->capacity;

	HashNode *current = salad->hash_table->table[index];
	while (current)
	{
		if (!strcmp(current->resource.file_path, file_path))
		{
			// printf("already loaded\n");
			if (current->resource.data)
				return &current->resource;
		}
		current = current->next;
	}

	if ((double)salad->hash_table->size / salad->hash_table->capacity >= LOAD_FACTOR_THRESHOLD)
	{
		ResizeHashTable();
	}

	HashNode *new_node = (HashNode *)malloc(sizeof(HashNode));
	if (!new_node)
	{
		HandleError("Failed to allocate memory for resource");
		return NULL;
	}
	new_node->resource = (Resource){0};
	strcpy(new_node->resource.file_path, file_path);
	new_node->resource.program_index = -1;
	new_node->resource.index = index;
	new_node->resource.data = NULL;
	size_t insert_index = Hash(file_path) % salad->hash_table->capacity;
	new_node->next = salad->hash_table->table[insert_index];
	salad->hash_table->table[insert_index] = new_node;
	salad->hash_table->size++;

	return &new_node->resource;
}

Resource *GetResource(int index)
{
	if (index < 0)
	{
		return NULL; // Invalid index or file path
	}
	Salad *salad = state->salad;
	HashNode *current = salad->hash_table->table[index % salad->hash_table->capacity];
	while (current)
	{
		if (current->resource.index == (int)index) // strcmp(current->resource.file_path, (const char *)index_or_file_path) == 0
		{
			return &(current->resource);
		}
		current = current->next;
	}

	return NULL;
}

void RemoveResource(int index)
{
	if (index < 0)
	{
		return; // Invalid index or file path
	}
	Salad *salad = state->salad;
	int hash_index = (int)index % (int)salad->hash_table->capacity;

	HashNode *current = salad->hash_table->table[hash_index];
	HashNode *prev = NULL;
	while (current)
	{
		if (current->resource.index == index)
		{
			if (prev)
				prev->next = current->next;
			else
				salad->hash_table->table[hash_index] = current->next;
			free(current);
			salad->hash_table->size--;
			return;
		}
		prev = current;
		current = current->next;
	}
}

// free all resources when done
void FreeResources()
{
	Salad *salad = state->salad;
	if (!salad || !salad->hash_table || !salad->hash_table->table)
		return;
	for (size_t i = 0; i < salad->hash_table->capacity; i++)
	{
		HashNode *current = salad->hash_table->table[i];
		while (current)
		{
			HashNode *next = current->next;
			free(current);
			current = next;
		}
	}

	free(salad->hash_table->table);
	free(salad->hash_table);
	free(salad);
}