/***************************************************************************
  tag: Peter Soetens  Mon Jan 19 14:11:20 CET 2004  PropertyMutatingIntrospection.hpp 

                        PropertyMutatingIntrospection.hpp -  description
                           -------------------
    begin                : Mon January 19 2004
    copyright            : (C) 2004 Peter Soetens
    email                : peter.soetens@mech.kuleuven.ac.be
 
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 
 
#ifndef PI_MUTATINGPROPERTYINTROSPECTION_HPP
#define PI_MUTATINGPROPERTYINTROSPECTION_HPP

#include <string>

namespace ORO_CoreLib
{

    class PropertyBag;
    class PropertyIntrospection;
	template<typename T> class Property;
    
	template<typename T>
	void decomposeProperty(PropertyIntrospection *m, Property<T> &b);

	/**
	 * An interface which all classes which wish to introspect
     * a Property should implement. When you call 
     * PropertyBase::introspect( PropertyIntrospection* ), The base
     * will call one of below methods to identify its type to the caller.
	 *
	 * @see Property
	 * @see PropertyBag
	 */
    class PropertyMutatingIntrospection
    {
        public:
			/**
			 * introspect a property of type bool.
			 * @param v The property to be introspectd.
			 */
			virtual void introspect(Property<bool> &v) = 0;
			
			/**
			 * introspect a property of type char.
			 * @param v The property to be introspectd.
			 */
			virtual void introspect(Property<char> &v) = 0;
			
			/**
			 * introspect a property of type int.
			 * @param v The property to be introspectd.
			 */
			virtual void introspect(Property<int> &v) = 0;
			
			/**
			 * introspect a property of type double.
			 * @param v The property to be introspectd.
			 */
			virtual void introspect(Property<double> &v) = 0;
			
			/**
			 * introspect a property of type string.
			 * @param v The property to be introspectd.
			 */
			virtual void introspect(Property<std::string> &v) = 0;
			
			/**
			 * introspect a property of type property bag. This enables
			 * nested property bags.
			 * @param v The property to be introspectd.
			 */
            virtual void introspect(Property<PropertyBag> &v) = 0;

            /** 
             * Unknown types must decompose theirselves into the primitives.
             * For each type you define, you need to provide the function
             * 
             *    decomposeProperty( PropertyIntrospection*, const Property<YourType>& )
             *
             * An Property<std::string> will be generated containing an error message
             * when this function is not found.
             */
            template< class T >
            void introspect( Property<T> &v )
            {
                decomposeProperty(this, v);
            }
	};
}

#endif
