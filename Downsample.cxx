#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkShrinkImageFilter.h"
#include "itkVectorImage.h"

typedef itk::VectorImage<float, 2> FloatImageType;
typedef itk::VectorImage<unsigned char, 2> UnsignedCharImageType;

std::string GetExtension(const std::string& filename);

template <typename TImage>
void ReadShrinkWrite(const std::string& inputFilename, const unsigned int shrinkFactor, const std::string& outputFilename);

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 4)
    {
    std::cerr << "Required: inputFilename outputFilename shrinkFactor" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];
  
  std::stringstream ss;
  ss << argv[3];
  unsigned int shrinkFactor;
  ss >> shrinkFactor;

  // Output arguments
  std::cout << "inputFilename " << inputFilename << std::endl;
  std::cout << "outputFilename " << outputFilename << std::endl;

  std::string extension = GetExtension(inputFilename);
  std::cout << "File extension: " << extension << std::endl;
  
  if(extension.compare("png") == 0)
    {
    ReadShrinkWrite<UnsignedCharImageType>(inputFilename, shrinkFactor, outputFilename);
    }
  else
    {
    ReadShrinkWrite<FloatImageType>(inputFilename, shrinkFactor, outputFilename);
    }
  return EXIT_SUCCESS;
}

std::vector<std::string> split(const std::string &s, char delim) 
{
  std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}
 
std::string GetExtension(const std::string& filename)
{
  std::vector<std::string> words = split(filename, '.');
  return words[words.size()-1];
}

template <typename TImage>
void ReadShrinkWrite(const std::string& inputFilename, const unsigned int shrinkFactor, const std::string& outputFilename)
{
  // Read file
  typedef itk::ImageFileReader<TImage> ReaderType;
  typename ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();

  // Compute absolute values
  typedef itk::ShrinkImageFilter<TImage, TImage > ShrinkFilterType;
  typename ShrinkFilterType::Pointer shrinkFilter = ShrinkFilterType::New();
  shrinkFilter->SetInput( reader->GetOutput() );
  shrinkFilter->Update();
  shrinkFilter->SetShrinkFactors(shrinkFactor);

  // Write the result
  typedef  itk::ImageFileWriter<TImage> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(shrinkFilter->GetOutput());
  writer->Update();
}
