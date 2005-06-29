#ifndef RELATION_H
#define RELATION_H

#include "SiconosConst.h"
#include "Interaction.h"
#include "RelationXML.h"
#include "DSInputOutput.h"
#include "check.h"

class Interaction;
class RelationXML;
class DSInputOutput;

/** \class Relation
 *  \brief this class represents relation laws (contact, ...) in an interaction between 2 DS;
 *  \author SICONOS Development Team - copyright INRIA
 *  \version 1.0
 *  \date (Creation) Apr 27, 2004
 *
 *
 *
 *   \warning
 */
class Relation
{
public:

  /** \fn Relation()
   *  \brief default constructor
   */
  Relation();

  /** \fn Relation(RelationXML*)
   *  \brief constructor with XML object of the Relation
   *  \param RelationXML* : the XML object corresponding
   */
  Relation(RelationXML*);

  virtual ~Relation();

  /** \fn inline RelationXML* getRelationXML()
   *  \brief allows to get the RelationXML* of the Relation
   *  \return a pointer on the RelationXML of the Relation
   */
  inline RelationXML* getRelationXML()
  {
    return relationxml;
  }

  /** \fn inline void setRelationXML(RelationXML *rxml)
   *  \brief allows to set the RelationXML* of the Relation
   *  \param RelationXML* : the pointer to set
   */
  inline void setRelationXML(RelationXML *rxml)
  {
    relationxml = rxml;
  }

  /** \fn Interaction* getInteraction(void)
   *  \brief allows to get the Interaction which contains this Relation
   *  \return a pointer on an Interaction
   */
  inline Interaction* getInteraction(void) const
  {
    return interaction;
  }

  /** \fn void setInteraction(Interaction* i)
   *  \brief allows to set the Interaction which contains this Relation
   */
  inline void setInteraction(Interaction* i)
  {
    interaction = i;
  }

  /** \fn inline string getType()
   *  \brief allows to get the type of the Relation
   *  \return string : the type of the Relation
   */
  inline const std::string  getType() const
  {
    return relationType;
  }

  /** \fn vector<DSInputOutput*> getDSInputOutputs(void)
   *  \brief allows to get all the DSInputOutput of the Relation
   *  \return the vector of DSInputOutput
   */
  std::vector<DSInputOutput*> getDSInputOutputs(void);

  /** \fn DSInputOutput* getDSInputOutput(const int&)
   *  \brief allows to get one specific DSInputOutput, with its place in the vector of DSInputOutput
   *  \param int : the place of the DSInputOutput in the vector of DSInputOutput of the Relation
   *  \return DSInputOutput* : dsioVector[ i ] DSInputOutput
   */
  DSInputOutput* getDSInputOutput(const unsigned int&);

  /** \fn void setDSInputOutputs(vector<DSInputOutput*>)
   *  \brief allows to set all the DSInputOutputs of the Relation
   *  \param vector<DSInputOutput*> : the vector to set
   */
  void setDSInputOutputs(std::vector<DSInputOutput*>);

  /** \fn void addDSInputOutput(DSInputOutput*)
   *  \brief allows to add the DSInputOutput to the Relation
   *  \param DSInputOutput* : the DSInputOutput to add
   */
  void addDSInputOutput(DSInputOutput*);

  //////////////////////////

  /** \fn void computeOutput(double time);
   *  \brief default function to compute y
   *  \param double : current time
   *  \exception RuntimeException
   */
  virtual void computeOutput(const double& time);

  /** \fn void computeFreeOutput(double time);
   *  \brief default function to compute y for the free state
   *  \param double : current time
   *  \exception RuntimeException
   */
  virtual void computeFreeOutput(const double& time);

  /** \fn void computeInput(double time);
   *  \brief default function to compute r
   *  \param double : current time
   *  \exception RuntimeException
   */
  virtual void computeInput(const double& time);

  /** \fn void setComputeOutputFunction(string pluginPath, string functionName)
   *  \brief allow to set a specified function to compute output
   *  \param string : the complete path to the plugin
   *  \param string : the function name to use in this plugin
   *  \exception SiconosSharedLibraryException
   */
  virtual void setComputeOutputFunction(const std::string&, const std::string&);

  /** \fn void setComputeInputFunction(string pluginPath, string functionName)
   *  \brief allow to set a specified function to compute output
   *  \param string : the complete path to the plugin
   *  \param string : the function name to use in this plugin
   *  \exception SiconosSharedLibraryException
   */
  virtual void setComputeInputFunction(const std::string&, const std::string&);

  ///////////////////////

protected:

  /** type of the Relation */
  std::string  relationType;

  /** the Interaction which contains this Relation */
  Interaction *interaction;

  /** the object linked this Relation to read XML data */
  RelationXML *relationxml;

  /** class for manage plugin (open, close librairy...) */
  SiconosSharedLibrary cShared;

  /* contains the name of the plugin used for computeInput */
  std::string  computeInputName;
  /* contains the name of the plugin used for computeOutput */
  std::string  computeOutputName;

  /** \fn void (*computeOutputPtr)(double* xPtr, double* time, double* lambdaPtr, double* yPtr)
   *  \brief computes y
   *  \param double* xPtr : the pointer to the first element of the vector x
   *  \param double* time : the current time
   *  \param double* lambdaPtr : the pointer to the first element of the vector lambda
   *  \param double* yPtr : the pointer to the first element of the vector y (in-out parameter)
   */
  void (*computeOutputPtr)(double* xPtr, double* time, double* lambdaPtr, double* yPtr);

  /** \fn void (*computeInputPtr)(double* xPtr, double* time, double* lambdaPtr, double* rPtr)
   *  \brief computes r
   *  \param double* xPtr : the pointer to the first element of the vector x
   *  \param double* time : the current time
   *  \param double* lambdaPtr : the pointer to the first element of the vector lambda
   *  \param double* rPtr : the pointer to the first element of the vector r (in-out parameter)
   */
  void (*computeInputPtr)(double* xPtr, double* time, double* lambdaPtr, double* rPtr);


private :
  /** contains a link to the DSInputOutput of the DynamicalSystems */
  std::vector<DSInputOutput*> dsioVector;
};

#endif // RELATION_H
