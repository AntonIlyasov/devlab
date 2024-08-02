transform_inverse = transform.inverse();




/**@brief Return the inverse of this transform */
Transform inverse() const
{ 
    Matrix3x3 inv = m_basis.transpose();        // m_basis - Storage for the rotation
    return Transform(inv, inv * -m_origin);     // m_origin - Storage for the translation
}


  /**@brief Constructor from Matrix3x3 (optional Vector3)
   * @param b Rotation from Matrix 
   * @param c Translation from Vector default (0,0,0)*/
	explicit TFSIMD_FORCE_INLINE Transform(const Matrix3x3& b, 
		const Vector3& c = Vector3(tfScalar(0), tfScalar(0), tfScalar(0)))
		: m_basis(b),
		m_origin(c)
	{}


	/** @brief Set the matrix from a quaternion
	*  @param q The Quaternion to match */  
	void setRotation(const Quaternion& q) 
	{
		tfScalar d = q.length2();
		tfFullAssert(d != tfScalar(0.0));
		tfScalar s = tfScalar(2.0) / d;
		tfScalar xs = q.x() * s,   ys = q.y() * s,   zs = q.z() * s;
		tfScalar wx = q.w() * xs,  wy = q.w() * ys,  wz = q.w() * zs;
		tfScalar xx = q.x() * xs,  xy = q.x() * ys,  xz = q.x() * zs;
		tfScalar yy = q.y() * ys,  yz = q.y() * zs,  zz = q.z() * zs;
		setValue(tfScalar(1.0) - (yy + zz), xy - wz, xz + wy,
			xy + wz, tfScalar(1.0) - (xx + zz), yz - wx,
			xz - wy, yz + wx, tfScalar(1.0) - (xx + yy));
	}


  /**@brief Return the length squared of the quaternion */
	tfScalar length2() const
	{
		return dot(*this);
	}

  	/** @brief Set the matrix from euler angles YPR around ZYX axes
	* @param eulerZ Yaw aboud Z axis
	* @param eulerY Pitch around Y axis
	* @param eulerX Roll about X axis
	* 
	* These angles are used to produce a rotation matrix. The euler
	* angles are applied in ZYX order. I.e a vector is first rotated 
	* about X then Y and then Z
	**/
	void setEulerYPR(tfScalar eulerZ, tfScalar eulerY,tfScalar eulerX)  { 
		tfScalar ci ( tfCos(eulerX)); 
		tfScalar cj ( tfCos(eulerY)); 
		tfScalar ch ( tfCos(eulerZ)); 
		tfScalar si ( tfSin(eulerX)); 
		tfScalar sj ( tfSin(eulerY)); 
		tfScalar sh ( tfSin(eulerZ)); 
		tfScalar cc = ci * ch; 
		tfScalar cs = ci * sh; 
		tfScalar sc = si * ch; 
		tfScalar ss = si * sh;

		setValue(cj * ch, sj * sc - cs, sj * cc + ss,
			cj * sh, sj * ss + cc, sj * cs - sc, 
			-sj,      cj * si,      cj * ci);
	}

	/** @brief Set the values of the matrix explicitly (row major)
	*  @param xx Top left
	*  @param xy Top Middle
	*  @param xz Top Right
	*  @param yx Middle Left
	*  @param yy Middle Middle
	*  @param yz Middle Right
	*  @param zx Bottom Left
	*  @param zy Bottom Middle
	*  @param zz Bottom Right*/
	void setValue(const tfScalar& xx, const tfScalar& xy, const tfScalar& xz, 
		const tfScalar& yx, const tfScalar& yy, const tfScalar& yz, 
		const tfScalar& zx, const tfScalar& zy, const tfScalar& zz)
	{
		m_el[0].setValue(xx,xy,xz);
		m_el[1].setValue(yx,yy,yz);
		m_el[2].setValue(zx,zy,zz);
	}
  
TFSIMD_FORCE_INLINE tfScalar tfCos(tfScalar x) { return cos(x); }
TFSIMD_FORCE_INLINE tfScalar tfSin(tfScalar x) { return sin(x); }