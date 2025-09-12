#include <stdio.h>

struct student
{
  int rollnum;
  char name[30];
  int age;
};

int save_num_text(char *filename)
{
  FILE *fp;
  fp = fopen(filename, "w");
  // Save text data into text file
  for (int i = 0; i < 20; i++)
  {
    fprintf(fp, "%d ", i);
  }
  fclose(fp);
  return 0;
}

int read_num_text(char *filename)
{
  FILE *fp;
  fp = fopen(filename, "r");
  int nums[20];
  for (int i = 0; i < 20; i++)
  {
    fscanf(fp, "%d", &nums[i]);
  }
  for (int i = 0; i < 20; i++)
  {
    printf("%d ", nums[i]);
  }
  fclose(fp);
  return 0;
  // write your code
}

int save_struct_text(char *filename)
{
  // write your code
  struct student s1 = {1, "A", 20};
  struct student s2 = {2, "B", 21};
  struct student s3 = {3, "C", 22};
  struct student s4 = {4, "D", 23};
  struct student s5 = {5, "E", 24};

  FILE *fp;
  fp = fopen(filename, "w");
  fprintf(fp, "%d %s %d\n", s1.rollnum, s1.name, s1.age);
  fprintf(fp, "%d %s %d\n", s2.rollnum, s2.name, s2.age);
  fprintf(fp, "%d %s %d\n", s3.rollnum, s3.name, s3.age);
  fprintf(fp, "%d %s %d\n", s4.rollnum, s4.name, s4.age);
  fprintf(fp, "%d %s %d\n", s5.rollnum, s5.name, s5.age);
  fclose(fp);
  return 0;
}

int read_struct_text(char *filename)
{
  // write your code
  FILE *fp;
  fp = fopen(filename, "r");
  for (int i = 0; i < 5; i++)
  {
    struct student s;
    fscanf(fp, "%d %s %d\n", &s.rollnum, s.name, &s.age);
    printf("Rollnum: %d, Name: %s, Age %d\n", s.rollnum, s.name, s.age);
  }
  return 0;
}

int save_num_binary(char *filename)
{
  // write your code
  FILE *fp;
  fp = fopen(filename, "wb");

  // Save integer data into binary file

  for (int i = 0; i < 20; i++)
  {
    fwrite(&i, sizeof(int), 1, fp);
  }
  // Close the file
  fclose(fp);
  return 0;
}

int read_num_binary(char *filename)
{
  // write your code
  FILE *fp;
  fp = fopen(filename, "rb");

  // Read int data from binary file in a loop
  // int bytesread;
  for (int i = 0; i < 20; i++)
  {
    int intvar;
    fread(&intvar, sizeof(int), 1, fp);
    printf("Integer from file: %d\n", intvar);
  }
  fclose(fp);
  return 0;
}

int save_struct_binary(char *filename)
{
  // write your code
  struct student s1 = {1, "A", 10};
  struct student s2 = {2, "B", 11};
  struct student s3 = {3, "C", 12};
  struct student s4 = {4, "D", 13};
  struct student s5 = {5, "E", 14};

  FILE *fp;
  fp = fopen(filename, "wb");
  fwrite(&s1, sizeof(struct student), 1, fp);
  fwrite(&s2, sizeof(struct student), 1, fp);
  fwrite(&s3, sizeof(struct student), 1, fp);
  fwrite(&s4, sizeof(struct student), 1, fp);
  fwrite(&s5, sizeof(struct student), 1, fp);
  fclose(fp);
}

int read_struct_binary(char *filename)
{
  // write your code
  FILE *fp;
  fp = fopen(filename, "rb");
  for (int i = 0; i < 5; i++)
  {
    struct student s;
    fread(&s, sizeof(struct student), 1, fp);
    printf("Rollnum: %d, Name: %s, Age %d\n", s.rollnum, s.name, s.age);
  }
  fclose(fp);
  return 0;
}
