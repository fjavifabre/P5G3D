#pragma once
#include <glm\glm.hpp>

class Mesh;

//!  Object class. 
/*!
The scene contains objects, which can have a mesh to display and execute logic.
*/
class Object
{
public:

	//! Default constructor.
	Object();

	//! Default destructor.
	~Object();


	//! Initialize object.
	/*!
	\param name Name of the object.
	\param parent Parent of the object.
	\param mesh Mesh of the object.
	*/
	void InitObject(char * name, Object *parent, Mesh *mesh);

	//! Set parent.
	/*!
	\param parent Link parent to the object.
	*/
	void SetParent(Object *parent);

	//! Get parent.
	/*!
	\returns Linked parent to the object.
	*/
	Object* GetParent();

	//! Set mesh.
	/*!
	\param mesh Link mesh to the object.
	*/
	void SetMesh(Mesh *mesh);

	//! Get mesh.
	/*!
	\returns Linked mesh to the object.
	*/
	Mesh* GetMesh();

	//! Get model matrix.
	/*!
	\returns Model matrix in use (default/standard).
	*/
	glm::mat4* GetModelMatrix();

	//! Update object.
	/*!
	Logic used for the object.
	\param dt Delta time.
	*/
	virtual void Update(float dt); //Metodo preparado para ser overrided, así se pueden crear nuevas clases de objetos con comportamientos especiales

	//! Set object position.
	/*!
	\param position Moves the object to a specified position in global space.
	*/
	void SetPosition(glm::vec3 position);

	//! Set object scale.
	/*!
	\param scale Scales the object.
	*/
	void SetScale(glm::vec3 scale);

	//! Set object rotaion.
	/*!
	Rotates the object using euler angles.
	\param eulerAngles Rotation of the object.
	*/
	void SetEulerRotation(glm::vec3 eulerAngles); //Usar roll, pitch y yaw

	//Para matrices modelo con transformaciones complejas (sesgado, rotar alrededor de un punto, etc)
	//! Use alternative model matrix.
	/*!
	\param use True: use the alternative model matrix. False: use the default model matrix.
	*/
	void UseAlternativeModelMat(bool use);

	//! Set alternative model matrix.
	/*!
	\param modelMat Sets the alternative model matrix of the object
	*/
	void SetAlternativeModelMat(glm::mat4 modelMat); 

	//! Set active object.
	/*!
	Activates or deactivates the object.
	\param active Sets the object active or not.
	\returns If the object is active or not.
	*/
	bool SetActive(bool active);

	//! Is active object.
	/*!
	\returns If the object is active or not.
	*/
	bool IsActive();

protected:
	char * m_name; //!< Name of the object

	Object* m_parent; //¿Crear herencia entre objetos para heredar transformaciones?
	Mesh* m_linkedMesh; //!< Mesh used by the object.

	bool m_active; //!< If the object is active or not for the updates.

	glm::vec3 m_globalPosition; //!< Global position of the object.
	glm::vec3 m_globalScale; //!< Scale of the object.
	glm::vec3 m_eulerAngles; //!< Rotation of the object in euler angles.

	glm::mat4 m_modelMat; //!< Model matrix of the object.
	glm::mat4 m_altModelMat; //!< Alternative model matrix for more complex transformations.

	bool m_useAltMat = false; //!< Use the default or the alternative model matrix.  \sa m_modelMat \sa m_altModelMat
	bool m_updateMatrix = false; //!< Update the matrix if necessary.

	//! Update model matrix.
	/*!
	Updates the model matrix if necessary.
	*/
	void UpdateMatrix();
};

