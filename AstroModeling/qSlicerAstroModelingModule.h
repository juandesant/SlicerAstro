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

#ifndef __qSlicerAstroModelingModule_h
#define __qSlicerAstroModelingModule_h

// SlicerQt includes
#include "qSlicerLoadableModule.h"

#include "qSlicerAstroModelingModuleExport.h"

class qSlicerAstroModelingModulePrivate;

/// \ingroup Slicer_QtModules_AstroModeling
class Q_SLICER_QTMODULES_ASTROMODELING_EXPORT qSlicerAstroModelingModule :
  public qSlicerLoadableModule
{
  Q_OBJECT
  Q_INTERFACES(qSlicerLoadableModule);

public:

  typedef qSlicerLoadableModule Superclass;
  explicit qSlicerAstroModelingModule(QObject *parent=0);
  virtual ~qSlicerAstroModelingModule();

  qSlicerGetTitleMacro(QTMODULE_TITLE);

  /// Return a custom icon for the module
  virtual QIcon icon()const;
  virtual QStringList categories() const;

  virtual QString helpText()const;
  virtual QString acknowledgementText()const;
  virtual QStringList contributors()const;

  virtual QStringList dependencies()const;

protected:
  /// Initialize the module. Register the volumes reader/writer
  virtual void setup();

  /// Create and return the widget representation associated to this module
  virtual qSlicerAbstractModuleRepresentation * createWidgetRepresentation();

  /// Create and return the logic associated to this module
  virtual vtkMRMLAbstractLogic* createLogic();

protected:
  QScopedPointer<qSlicerAstroModelingModulePrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerAstroModelingModule);
  Q_DISABLE_COPY(qSlicerAstroModelingModule);

};

#endif
