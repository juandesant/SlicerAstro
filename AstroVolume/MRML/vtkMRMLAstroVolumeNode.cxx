/*==============================================================================

  Copyright (c) Kapteyn Astronomical Institute
  University of Groningen, Groningen, Netherlands. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Davide Punzo, Kapteyn Astronomical Institute,
  and was supported through the European Research Consil grant nr. 291531.

==============================================================================*/

#include <string>
#include <cstdlib>
#include <math.h>

// VTK includes
#include <vtkImageData.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>

// MRML includes
#include <vtkMRMLAstroLabelMapVolumeNode.h>
#include <vtkMRMLAstroVolumeDisplayNode.h>
#include <vtkMRMLAstroVolumeNode.h>
#include <vtkMRMLAstroVolumeStorageNode.h>
#include <vtkMRMLVolumeNode.h>

//----------------------------------------------------------------------------
vtkMRMLNodeNewMacro(vtkMRMLAstroVolumeNode);

//----------------------------------------------------------------------------
vtkMRMLAstroVolumeNode::vtkMRMLAstroVolumeNode()
{
  this->SetAttribute("SlicerAstro.PresetsActive", "0");
}

//----------------------------------------------------------------------------
vtkMRMLAstroVolumeNode::~vtkMRMLAstroVolumeNode()
{
}

namespace
{
//----------------------------------------------------------------------------
template <typename T> std::string NumberToString(T V)
{
  std::string stringValue;
  std::stringstream strstream;
  strstream << V;
  strstream >> stringValue;
  return stringValue;
}

//----------------------------------------------------------------------------
std::string DoubleToString(double Value)
{
  return NumberToString<double>(Value);
}

//----------------------------------------------------------------------------
template <typename T> T StringToNumber(const char* num)
{
  std::stringstream ss;
  ss << num;
  T result;
  return ss >> result ? result : 0;
}

//----------------------------------------------------------------------------
int StringToInt(const char* str)
{
  return StringToNumber<int>(str);
}
}// end namespace

//----------------------------------------------------------------------------
void vtkMRMLAstroVolumeNode::ReadXMLAttributes(const char** atts)
{
  this->Superclass::ReadXMLAttributes(atts);

  this->WriteXML(std::cout,0);
}

//----------------------------------------------------------------------------
void vtkMRMLAstroVolumeNode::WriteXML(ostream& of, int nIndent)
{
  this->Superclass::WriteXML(of, nIndent);
}

//----------------------------------------------------------------------------
void vtkMRMLAstroVolumeNode::Copy(vtkMRMLNode *anode)
{
  vtkMRMLAstroVolumeNode *astroVolumeNode = vtkMRMLAstroVolumeNode::SafeDownCast(anode);
  if (!astroVolumeNode)
    {
    return;
    }

  this->Superclass::Copy(astroVolumeNode);
}

//----------------------------------------------------------------------------
void vtkMRMLAstroVolumeNode::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

//---------------------------------------------------------------------------
vtkMRMLAstroVolumeDisplayNode* vtkMRMLAstroVolumeNode::GetAstroVolumeDisplayNode()
{
  return vtkMRMLAstroVolumeDisplayNode::SafeDownCast(this->GetDisplayNode());
}

//---------------------------------------------------------------------------
void vtkMRMLAstroVolumeNode::UpdateRangeAttributes()
{
  this->GetImageData()->Modified();
  int *dims = this->GetImageData()->GetDimensions();
  int numElements = dims[0] * dims[1] * dims[2];
  const int DataType = this->GetImageData()->GetPointData()->GetScalars()->GetDataType();
  double max = this->GetImageData()->GetScalarTypeMin(), min = this->GetImageData()->GetScalarTypeMax();
  short *outSPixel = NULL;
  float *outFPixel = NULL;
  double *outDPixel = NULL;

  switch (DataType)
    {
  case VTK_SHORT:
    outSPixel = static_cast<short*> (this->GetImageData()->GetScalarPointer());
    for (int elementCnt = 0; elementCnt < numElements; elementCnt++)
      {
      if (*(outSPixel + elementCnt) > max)
        {
        max =  *(outSPixel + elementCnt);
        }
      if (*(outSPixel + elementCnt) < min)
        {
        min =  *(outSPixel + elementCnt);
        }
      }
    break;
    case VTK_FLOAT:
      outFPixel = static_cast<float*> (this->GetImageData()->GetScalarPointer());
      for (int elementCnt = 0; elementCnt < numElements; elementCnt++)
        {
        if (*(outFPixel + elementCnt) > max)
          {
          max =  *(outFPixel + elementCnt);
          }
        if (*(outFPixel + elementCnt) < min)
          {
          min =  *(outFPixel + elementCnt);
          }
        }
      break;
    case VTK_DOUBLE:
      outDPixel = static_cast<double*> (this->GetImageData()->GetScalarPointer());
      for (int elementCnt = 0; elementCnt < numElements; elementCnt++)
        {
        if (*(outDPixel + elementCnt) > max)
          {
          max =  *(outDPixel + elementCnt);
          }
        if (*(outDPixel + elementCnt) < min)
          {
          min =  *(outDPixel + elementCnt);
          }
        }
      break;
    default:
      vtkErrorMacro("vtkMRMLAstroVolumeNode::UpdateRangeAttributes() : Attempt to allocate scalars of type not allowed");
      return;
    }

  this->SetAttribute("SlicerAstro.DATAMAX", DoubleToString(max).c_str());
  this->SetAttribute("SlicerAstro.DATAMIN", DoubleToString(min).c_str());

  outSPixel = NULL;
  outFPixel = NULL;
  outDPixel = NULL;
  delete outSPixel;
  delete outFPixel;
  delete outDPixel;
}

//---------------------------------------------------------------------------
void vtkMRMLAstroVolumeNode::UpdateNoiseAttributes()
{
  //We calculate the noise as the std of 6 slices of the datacube.
  int *dims = this->GetImageData()->GetDimensions();
  const int DataType = this->GetImageData()->GetPointData()->GetScalars()->GetDataType();
  float *outFPixel = NULL;
  double *outDPixel = NULL;
  switch (DataType)
    {
    case VTK_FLOAT:
      outFPixel = static_cast<float*> (this->GetImageData()->GetScalarPointer(0,0,0));
      break;
    case VTK_DOUBLE:
      outDPixel = static_cast<double*> (this->GetImageData()->GetScalarPointer(0,0,0));
      break;
    default:
      vtkErrorMacro("vtkMRMLAstroVolumeNode::UpdateNoiseAttributes : Attempt to allocate scalars of type not allowed");
      return;
    }
  double sum = 0., noise1 = 0., noise2 = 0, noise = 0., mean1 = 0., mean2 = 0., mean = 0.;
  int lowBoundary;
  int highBoundary;

  if (StringToInt(this->GetAttribute("SlicerAstro.NAXIS")) == 3)
    {
    lowBoundary = dims[0] * dims[1] * 2;
    highBoundary = dims[0] * dims[1] * 4;
    }
  else if (StringToInt(this->GetAttribute("SlicerAstro.NAXIS")) == 2)
    {
    lowBoundary = dims[0] * 2;
    highBoundary = dims[0] * 4;
    }
  else
    {
    lowBoundary = 2;
    highBoundary = 4;
    }

  int cont = highBoundary - lowBoundary;

  switch (DataType)
    {
    case VTK_FLOAT:
      for( int elemCnt = lowBoundary; elemCnt <= highBoundary; elemCnt++)
        {
        sum += *(outFPixel + elemCnt);
        }
      sum /= cont;
      for( int elemCnt = lowBoundary; elemCnt <= highBoundary; elemCnt++)
        {
        noise1 += (*(outFPixel + elemCnt) - sum) * (*(outFPixel+elemCnt) - sum);
        }
      noise1 = sqrt(noise1 / cont);
      break;
    case VTK_DOUBLE:
      for( int elemCnt = lowBoundary; elemCnt <= highBoundary; elemCnt++)
        {
        sum += *(outDPixel + elemCnt);
        }
      sum /= cont;
      for( int elemCnt = lowBoundary; elemCnt <= highBoundary; elemCnt++)
        {
        noise1 += (*(outDPixel + elemCnt) - sum) * (*(outDPixel+elemCnt) - sum);
        }
      noise1 = sqrt(noise1 / cont);
      break;
    }

  mean1 = sum;

  if (StringToInt(this->GetAttribute("SlicerAstro.NAXIS")) == 3)
    {
    lowBoundary = dims[0] * dims[1] * (dims[2] - 4);
    highBoundary = dims[0] * dims[1] * (dims[2] - 2);
    }
  else if (StringToInt(this->GetAttribute("SlicerAstro.NAXIS")) == 2)
    {
    lowBoundary = dims[0] * (dims[1] - 4);
    highBoundary = dims[0] * (dims[1] - 2);
    }
  else
    {
    lowBoundary = dims[0] - 4;
    highBoundary = dims[0] - 2;
    }

  sum = 0.;

  switch (DataType)
    {
    case VTK_FLOAT:
      for( int elemCnt = lowBoundary; elemCnt <= highBoundary; elemCnt++)
        {
        sum += *(outFPixel + elemCnt);
        }
      sum /= cont;
      for( int elemCnt = lowBoundary; elemCnt <= highBoundary; elemCnt++)
        {
        noise2 += (*(outFPixel + elemCnt) - sum) * (*(outFPixel+elemCnt) - sum);
        }
      noise2 = sqrt(noise2 / cont);
      break;
    case VTK_DOUBLE:
      for( int elemCnt = lowBoundary; elemCnt <= highBoundary; elemCnt++)
        {
        sum += *(outDPixel + elemCnt);
        }
      sum /= cont;
      for( int elemCnt = lowBoundary; elemCnt <= highBoundary; elemCnt++)
        {
        noise2 += (*(outDPixel + elemCnt) - sum) * (*(outDPixel+elemCnt) - sum);
        }
      noise2 = sqrt(noise2 / cont);
      break;
    }

  mean2 = sum;

  noise = (noise1 + noise2) * 0.5;
  mean = (mean1 + mean2) * 0.5;

  this->SetAttribute("SlicerAstro.RMS", DoubleToString(noise).c_str());\
  this->SetAttribute("SlicerAstro.NOISEMEAN", DoubleToString(mean).c_str());
  outFPixel = NULL;
  outDPixel = NULL;
  delete outFPixel;
  delete outDPixel;
}

//-----------------------------------------------------------
void vtkMRMLAstroVolumeNode::CreateNoneNode(vtkMRMLScene *scene)
{
  // Create a None volume RGBA of 0, 0, 0 so the filters won't complain
  // about missing input
  vtkNew<vtkImageData> id;
  id->SetDimensions(1, 1, 1);
  id->AllocateScalars(VTK_DOUBLE, 4);
  id->GetPointData()->GetScalars()->FillComponent(0, 0.0);
  id->GetPointData()->GetScalars()->FillComponent(1, 125.0);
  id->GetPointData()->GetScalars()->FillComponent(2, 0.0);
  id->GetPointData()->GetScalars()->FillComponent(3, 0.0);

  vtkNew<vtkMRMLAstroVolumeNode> n;
  n->SetName("None");
  // the scene will set the id
  n->SetAndObserveImageData(id.GetPointer());
  scene->AddNode(n.GetPointer());
}

//---------------------------------------------------------------------------
vtkMRMLStorageNode* vtkMRMLAstroVolumeNode::CreateDefaultStorageNode()
{
  return vtkMRMLAstroVolumeStorageNode::New();
}

//---------------------------------------------------------------------------
void vtkMRMLAstroVolumeNode::CreateDefaultDisplayNodes()
{
  vtkMRMLAstroVolumeDisplayNode *displayNode = 
    vtkMRMLAstroVolumeDisplayNode::SafeDownCast(this->GetDisplayNode());
  if(displayNode == NULL)
    {
    displayNode = vtkMRMLAstroVolumeDisplayNode::New();
    if(this->GetScene())
      {
      displayNode->SetScene(this->GetScene());
      this->GetScene()->AddNode(displayNode);
      displayNode->SetDefaultColorMap();
      displayNode->Delete();

      this->SetAndObserveDisplayNodeID(displayNode->GetID());
      std::cout << "Display node set and observed" << std::endl;
      }
    }
}
