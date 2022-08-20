////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Saxonica Limited.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
// This Source Code Form is "Incompatible With Secondary Licenses", as defined by the Mozilla Public License, v. 2.0.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef SAXON_XDMNODE_h
#define SAXON_XDMNODE_h

#include "XdmItem.h"
#include <string.h>

typedef enum eXdmNodeKind { DOCUMENT = 9, ELEMENT = 1, ATTRIBUTE = 2, TEXT = 3, COMMENT = 8, PROCESSING_INSTRUCTION = 7, NAMESPACE = 13, UNKNOWN = 0 } XDM_NODE_KIND;



class XdmValue;

/**
 * This class represents a node in the XDM data model. A Node is an <code>XdmItem</code>, and is therefore an
 * <code>XdmValue</code> in its own right, and may also participate as one item within a sequence value.
 * <p>The XdmNode interface exposes basic properties of the node, such as its name, its string value, and
 * its typed value. </p>
 * <p>Note that node identity cannot be inferred from object identity. The same node may be represented
 * by different <code>XdmNode</code> instances at different times, or even at the same time. The equals()
 * method on this class can be used to test for node identity.</p>
 *
 */
class XdmNode : public XdmItem
{

public:

    //! XdmNode constructor created as a wrapper around an existing Saxon Java object
    /**
     * XdmNode constructor created from a JNI Java object. This is for internal use only
     * @param obj - The internal Java object for the XdmNode is wrapped here in the C++ XdmNode
     */
	XdmNode(jobject);

    //! XdmNode constructor created as a wrapper around an existing Saxon Java object
    /**
     * XdmNode constructor created from a JNI Java object. This is for internal use only
     * @param parent - The parent XdmNode to this node object
     * @param obj - The internal Java object for the XdmNode is wrapped here in the C++ XdmNode
     * @param kind - The  kind of node, for example <code>XdmNodeKind#ELEMENT</code> or <code>XdmNodeKind#ATTRIBUTE</code>
     */
	XdmNode(XdmNode *parent, jobject, XDM_NODE_KIND kind);

    //! Destructor.
    /**
     * Destructor for XdmNode
     */
	virtual ~XdmNode();

    //! Determine whether the item is an atomic value or some other type of item
    /**
     *
     *
     * @return true if the item is an atomic value, false if it is a node or a function (including maps and arrays)
     */
	virtual bool isAtomic();

	//! Get the first item in the sequence
	 /**
         *
         * @return XdmItem or nullptr if sequence is empty
         */
     XdmItem * getHead();


     //! Get the kind of node.
    /**
     *
     *
     * @return the kind of node, for example <code>XdmNodeKind#ELEMENT</code> or <code>XdmNodeKind#ATTRIBUTE</code>
     */
	XDM_NODE_KIND getNodeKind();

	//! Get the name of the node, as a string in the form of a EQName
	/**
	 *
	 *
	 * @return the name of the node. In the case of unnamed nodes (for example, text and comment nodes)
	 *         return NULL.
	 */
	const char * getNodeName();



	//! Get the local name of the node
	/**
	 *
	 *
	 * @return the name of the node. In the case of unnamed nodes (for example, text and comment nodes)
	 *         return nullptr.
	 */
	const char * getLocalName();

	//! Get the typed value of this node, as defined in XDM
	/**
	 *
	 *
	 * @return the typed value. If the typed value is a single atomic value, this will be returned
	 * as an instance of <code>XdmAtomicValue</code>
	 */
	XdmValue * getTypedValue();

	//! Get the base URI of this node
    /**
     *
     * @return the base URI, as defined in the XDM model. The value may be null if no base URI is known for the
     *         node, for example if the tree was built from a StreamSource with no associated URI, or if the node has
     *         no parent.
     */
	const char* getBaseUri();

	//! Get the string value of the item. For a node, this gets the string value of the node.
	/**
	 * For an atomic value, it has the same effect as casting the value
	 * to a string. In all cases the result is the same as applying the XPath string()
	 * function.
	 * <p>For atomic values, the result is the same as the result of calling
	 * <code>toString</code>. This is not the case for nodes, where <code>toString</code>
	 * returns an XML serialization of the node.</p>
	 *
	 * @return the result of converting the item to a string.
	 * @remark the SaxonProcessor argument has been removed from release version 1.2.1
	 */
	const char * getStringValue();


	//! The toString() method returns a simple XML serialization of the node with defaulted serialization parameters.
    /**
     *
     * <p>In the case of an element node, the result will be a well-formed
     * XML document serialized as defined in the W3C XSLT/XQuery serialization specification,
     * using options method="xml", indent="yes", omit-xml-declaration="yes".</p>
     * <p>In the case of a document node, the result will be a well-formed
     * XML document provided that the document node contains exactly one element child,
     * and no text node children. In other cases it will be a well-formed external
     * general parsed entity.</p>
     * <p>In the case of an attribute node, the output is a string in the form
     * <code>name="value"</code>. The name will use the original namespace prefix.</p>
     * <p>In the case of a namespace node, the output is a string in the form of a namespace
     * declaration, that is <code>xmlns="uri"</code> or <code>xmlns:pre="uri"</code>.</p>
     * <p>Other nodes, such as text nodes, comments, and processing instructions, are
     * represented as they would appear in lexical XML. Note: this means that in the case
     * of text nodes, special characters such as <code>&amp;</code> and <code>&lt;</code>
     * are output in escaped form. To get the unescaped string value of a text node, use
     * <code>getStringValue()</code> instead.</p>
     *
     * @return a simple XML serialization of the node. Under error conditions the method
     * may return an error message which will always begin with the label "Error: ".
     */
	const char * toString();


	//! Get the parent of this node
    /**
     *
     *
     * @return the parent of this node (a document or element node), or NULL if this node has no parent.
     */
	XdmNode* getParent();


	//! Get the string value of a named attribute (in no namespace) of this element
    /**
     *
     *
     * @param name the name of the required attribute, interpreted as a no-namespace name
     * @return NULL if this node is not an element, or if this element has no
     * attribute with the specified name. Otherwise return the string value of the
     * selected attribute node.
     */
	const char* getAttributeValue(const char *name);

	//! Get the number of attribute node for this current node
	/**
	 *
	 * @return returns the number of attributes on this node, other return zero if this node has no attributes or is not an element.
	 */
	int getAttributeCount();

    //! Get array of attribute nodes of this element
    /**
     *
     *
     * @param cache - true to cache the attribute nodes for future use. User os responsible for memory management for the attribute nodes.
     * @return NULL if this node is not an element, or if this element has no
     * attribute with the specified name. Otherwise return the attribute nodes as a pointer array.
     */
	XdmNode** getAttributeNodes(bool cache=false);

	//! Get the underlying JNI Java object for the XdmNode
	/**
	 *
	 * @return jobject - The unwrapped JNI object for the XdmNode
	 */
	jobject getUnderlyingValue() {

		return XdmItem::getUnderlyingValue();

	}

	//! Determine whether the item is a node or some other type of item
    /**
     *
     *
     * @return true if the item is a node, false if it is an atomic value or a function (including maps and arrays)
     */
    bool isNode(){
        return true;
    }



    //! Get all the child nodes from the current parent node
    /**
    *
    * @return pointer array of XdmNode objects
    */

	XdmNode** getChildren(bool cache = false);



	//! Get the ith child nodes from the current parent node
    /**
    *
    * @return a pointer to the ith child node
    */

    XdmNode* getChild(int i, bool cache = false);


    //! Get the count of child nodes from thie current node
    /**
     *
     * @return int for the count of child nodes
     */
	int getChildCount();


    //! Get the type of the object
    /**
     *
     * @return XDM_TYPE - Type of the XdmItem
     */
	XDM_TYPE getType() {
		return XDM_NODE;
	}





private:
	const char * baseURI; /*!< The base URI of this node*/
	const char * nodeName; /*!< The name of the node, as a clark name format*/
	const char * localName; /*!< The local part of the name */
	XdmNode ** children; /*!< Cached. Child nodes when getChildren method is first called; */
	int childCount; /*!< The number of of child nodes on the current node*/
	XdmNode * parent; /*!< Cached. The parent node of the current node.*/
	XdmValue * typedValue; /*!< Cached.  The typed value of this node, as defined in XDM */
	XdmNode ** attrValues; /*!< Cached attribute nodes when getAttributeNodes method is first called; */
	int attrCount; /*!< The number of attribute nodes from the current node */
	XDM_NODE_KIND nodeKind; /*!< The node kind for the current  node*/

};

#endif
