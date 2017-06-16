#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "gason.hpp"

#if 1
#define DO_OUT
#endif

using namespace gason;

void iterate(JsonValue o);

/////////////////////////////////////////////////////////////////////////////////////////////////////
//main
/////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
  char * buffer = 0;
  long length;
  FILE * f = fopen("data/test.json", "rb");

  if (!f)
  {
    return 1;
  }

  fseek(f, 0, SEEK_END);
  length = ftell(f);
  fseek(f, 0, SEEK_SET);
  buffer = (char*)malloc(length);
  if (buffer)
  {
    fread(buffer, 1, length, f);
  }
  fclose(f);


  JsonAllocator    allocator;
  JsonValue        root;
  JsonParseStatus  status = jsonParse(buffer, root, allocator);

  if (status != JSON_PARSE_OK)
  {

  }

  iterate(root);

  free(buffer);
  return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//iterate
/////////////////////////////////////////////////////////////////////////////////////////////////////

void iterate(JsonValue o)
{
  switch (o.getTag())
  {
  case GASON_NUMBER:
#ifdef DO_OUT
    printf("%g\n", o.toNumber());
#endif
    break;
  case GASON_STRING:
#ifdef DO_OUT
    printf("\"%s\"\n", o.toString());
#endif
    break;
  case GASON_ARRAY:
    for (JsonNode *node = o.toNode(); node != nullptr; node = node->next)
    {
      iterate(node->value);
    }
    break;
  case GASON_OBJECT:
    for (JsonNode *node = o.toNode(); node != nullptr; node = node->next)
    {
      printf("%s = ", node->key);
      iterate(node->value);
    }
    break;
  case GASON_TRUE:
#ifdef DO_OUT
    fprintf(stdout, "true\n");
#endif
    break;
  case GASON_FALSE:
#ifdef DO_OUT
    fprintf(stdout, "false\n");
#endif
    break;
  case GASON_NULL:
#ifdef DO_OUT
    printf("null\n");
#endif
    break;
  }
  return;
}
