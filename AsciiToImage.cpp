#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionConstIterator.h"

#include <iostream>
#include <fstream>

typedef itk::Image<float, 2> ImageType;

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 3)
    {
    std::cerr << "Required: inputFilename outputFileName" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];

  // Output arguments
  std::cout << "inputFilename " << inputFilename << std::endl;
  std::cout << "outputFilename " << outputFilename << std::endl;

  // Open file for input
  std::ifstream ifs(inputFilename.c_str());

  std::string line; // string to contain each line
  
  std::vector<std::vector<float> > rows;
  while(std::getline(ifs, line))
  {
    std::vector<float> row;

    std::stringstream ss;
    ss << line;
    std::cout << "line: " << line << std::endl;
    float value = 0.0f;
    while(ss >> value)
    {
      row.push_back(value);
    }
    std::cout << "row " << rows.size() << " has size " << row.size() << std::endl;
    rows.push_back(row);
  }

  std::cout << "There are " << rows.size() << " in the text file." << std::endl;
  
  // Count valid rows
  unsigned int numberOfValidRows = 0;
  for(unsigned int row = 0; row < rows.size(); ++row)
  {
    if(rows[row].size() == rows[0].size())
    {
      numberOfValidRows++;
    }
  }

  std::cout << "There are " << numberOfValidRows << " valid rows." << std::endl;

  unsigned int numberOfColumns = rows[0].size();

  std::cout << "There are " << numberOfColumns << " columns." << std::endl;
  
  // Construct image
  ImageType::Pointer image = ImageType::New();
  itk::Index<2> corner = {{0,0}};
  itk::Size<2> size = {{numberOfColumns, numberOfValidRows}};
  std::cout << "size: " << size << std::endl;
  
  itk::ImageRegion<2> region(corner, size);
  image->SetRegions(region);
  image->Allocate();

  for(int row = 0; row < static_cast<int>(numberOfValidRows); ++row)
  {
    for(int col = 0; col < static_cast<int>(numberOfColumns); ++col)
    {
    //itk::Index<2> index = {{row, col}}; // This is wrong
    itk::Index<2> index = {{col, row}};
    image->SetPixel(index, rows[row][col]);
    }
  }

  // Write file
  typedef itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(image);
  writer->Update();

  return EXIT_SUCCESS;
}
