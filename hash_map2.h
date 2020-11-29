#ifndef HASH_MAP
#define HASH_MAP

#include <algorithm>
#include <vector>
#include <list>

using namespace std;

template<class Key, class T>
struct value_type
{

        Key first;
        T second;
        bool occupied,
             marked_for_removal;

            value_type()
            {
               occupied = false;
                marked_for_removal = false;
            } // default constructor


            value_type (const value_type& p)
            {
                first = p.first;
                second = p.second;
                occupied = p.occupied;
                marked_for_removal = p.marked_for_removal;
            }

            value_type (const Key& key, const T& t): first (key)
            {
                 second = t;
                 occupied = false;
                 marked_for_removal = false;
            }

            bool operator== (const value_type& x)
            {
                return first == x.first;
            }
    }; // struct value_type

template<class Key, class T, class HashFunc >
class hash_map   // open addressing, offset of 1
{
    typedef Key key_type;

    typedef HashFunc hash_func;   // hashes key to unsigned long



    protected:

        const static int DEFAULT_SIZE;
        const static float MAX_RATIO;

        value_type<key_type, T> * buckets;
        int count,   // number of items in hash_map
            length,
			collisions;  // number of buckets in hash_map
        hash_func hash;





// Postcondition: the smallest prime at least twice as large as p has been returned.
int nextPrime (int p) {

        	int i;

        	bool factorFound;

        	p = 2 * p + 1;
        	while (true) {

            	i = 3;
            	factorFound = false;

					// Check 3, 5, 7, 9, ..., sqrt (p) for a factor of p
            	while (i * i < p && !factorFound)
                		if (p % i == 0)
                    			factorFound = true;
                		else
                    			i += 2;
            	if (!factorFound)
                		return p;
            	p += 2;  // get the next candidate for primality

        	} // while prime not found

        } // method nextPrime

        void check_for_expansion()
        {
            unsigned long hash_int;
            int index,
                old_length;

            if (count > int (MAX_RATIO * length))
            {
                value_type<key_type, T> * temp_buckets = buckets;

                old_length = length;
                length = nextPrime (old_length);
                buckets = new value_type<key_type, T> [length];
                for (int i = 0; i < old_length; i++)
                    if (temp_buckets [i].occupied)
                    {
                        hash_int = hash (temp_buckets [i].first);
                        index = hash_int % length;
                        while (buckets [index].occupied)
                            index = (index + 1) % length;
                        buckets [index] = temp_buckets [i];
                    } // posting temp_buckets [i] back into buckets
                    delete[] temp_buckets;
             } // doubling buckets size
        } // method check_for_expansion

    public:

        // Postcondition: this hash_map has been intialized.
        hash_map()
        {
            count = 0;
			collisions = 0;
            length = DEFAULT_SIZE;
            buckets = new value_type<key_type, T> [DEFAULT_SIZE];
        } // default constructor


        // Postcondition: the number of items in this hash_map has been returned.
        int size()
        {
             return count;
        } // method size

		int collisionCounter() {
			return collisions;
		}

        class iterator
        {
             friend class hash_map<key_type, T, hash_func>;

             protected:

                  unsigned index,
                           length;
                  value_type<key_type, T>* buckets;

             public:

                  bool operator== (const iterator& other) const
                  {
                      return (index == other.index) &&
                             (length == other.length) &&
                             (buckets == other.buckets);
                  } // ==

                  bool operator!= (const iterator& other) const
                  {
                      return !(*this == other);
                  } // !=

                  iterator operator++ (int)
                  {
                      iterator old_itr = *this;
                      do
                          index++;
                      while (!buckets [index].occupied && index < length);
                      return old_itr;
                  } // postincrement operator++


                  value_type<key_type, T>& operator* ()
                  {
                      return buckets [index];
                  } // operator*
        }; // iterator


        // Postcondition: an iterator positioned at the beginning
        //                of this hash_map has been returned.
        iterator begin()
        {
            if (size() == 0)
                return end();
            int i = 0;
            iterator itr;

            itr.buckets = buckets;
            while (!buckets [i].occupied && i < length - 1)
                i++;
            itr.index = i;
            itr.length = length;
            return itr;
        } // begin


        // Postcondition: an iterator has been returned that can be used
        //                in comparisons to terminate iterating through
        //                this hash_map.
        iterator end()
        {
            iterator itr;

            itr.index = length;
            itr.length = length;
            itr.buckets = buckets;
            return itr;
        } // end


        // Postcondition: If an item with x's key had already been inserted
        //                into this hash_map, the pair returned consists of an
        //                iterator positioned at the previously inserted item,
        //                and false.  Otherwise, the pair returned consists of
        //                an iterator positioned at the newly inserted item,
        //                and true.  If the Uniform Hashing Assumption holds,
        //                the averageTime (n, m) is constant.  The
        //                worstTime (n, m) is O (n).
        pair<iterator, bool> insert (const value_type<const key_type, T>& x)
        {
            key_type key = x.first;
            unsigned long hash_int = hash (key);
            int   index = hash_int % length;
            while ((buckets [index].marked_for_removal) ||
                    buckets [index].occupied && key != buckets [index].first)
                index = (index + 1) % length;
			if (buckets[index].occupied && key == buckets[index].first)
			{
				collisions++;
				return pair <iterator, bool>(find(key), false);
			}
            buckets [index].first = key;
            buckets [index].second = x.second;
            buckets [index].occupied = true;
            buckets [index].marked_for_removal = false;
            count++;

            check_for_expansion();
            return pair<iterator, bool> (find (key), true);
        } // insert


        // Postcondition: if this hash_map already contains a value whose
        //                key part is key, a reference to that value's
        //                second component has been returned.  Otherwise,
        //                a new value, <key, T()>, is inserted into this
        //                map and a reference to that value's second
        //                component has been returned. The averageTime (n)
        //                is constant, and worstTime (n) is O (n).
        T& operator[] (const key_type& key)
        {
            return (*(insert(value_type<const key_type, T> (key, T())).first)).second;
        } // operator[ ]


        // Postcondition: if this hash_map contains a value whose key
        //                component is key, an iterator positioned at that
        //                value has been returned.  Otherwise, an iterator
        //                with the same value as end() has been returned.
        //                The averageTime (n) is constant, and the
        //                worstTime (n) is O (n).
        iterator find (const key_type& key)
        {   
            unsigned long hash_int = hash (key);
            int index = hash_int % length;
            iterator itr;
            while (buckets [index].marked_for_removal
                      || (buckets [index].occupied
                          && buckets [index].first != key))
                index = (index + 1) % length;
            if (!buckets [index].occupied &&
                     !buckets [index].marked_for_removal)
                return end();
            itr.buckets = buckets;
            itr.index = index;
            itr.length = length;
            return itr;
        } // find


        // Precondition: itr is positioned at value in this hash_map.
        // Postcondition: the value that itr is positioned at has been
        //                deleted from this hash_map. If the Uniform
        //                Hashing Assumption holds, the averageTime (n, m)
        //                is constant.  The worstTime (n, m) is O (n).
        void erase (iterator itr)
        {
            buckets [itr.index].occupied = false;
            buckets [itr.index].marked_for_removal = true;
            count--;
        } // erase


        // Postcondition: the space allocated for this hash_map
        //                object has been deallocated.
        ~hash_map()
        {
                delete[] buckets;
        } // destructor
}; // hash_map

template<class Key, class T, class HashFunc>
const int hash_map<Key, T, HashFunc>::DEFAULT_SIZE = 211;

template<class Key, class T, class HashFunc>
const float hash_map<Key, T, HashFunc>::MAX_RATIO = 0.75;

#endif
