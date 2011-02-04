#ifndef HAT_TRIE_NODE_H
#define HAT_TRIE_NODE_H

namespace stx {

template <int alphabet_size, int (*indexof)(char)>
class hat_trie;

template <int alphabet_size, int (*indexof)(char)>
class hat_trie_container;

template <int alphabet_size, int (*indexof)(char)>
class hat_trie_node;

template <int alphabet_size, int (*indexof)(char)>
class hat_trie_node_base {
    friend class hat_trie<alphabet_size, indexof>;

  private:
    typedef hat_trie_node<alphabet_size, indexof> node;
    typedef hat_trie_container<alphabet_size, indexof> container;

  public:
    hat_trie_node_base(char ch = '\0') : _ch(ch), parent(NULL) {
        // TODO set_word(false);
    }
    virtual ~hat_trie_node_base() { }

    // accessors
    virtual bool word() const = 0;
    char ch() const { return _ch; }

    // modifiers
    virtual void set_word(bool b) = 0;

  protected:
    char _ch;
    node *parent;
};

// -----------------------
// hat trie helper classes
// -----------------------

template <int alphabet_size, int (*indexof)(char)>
class hat_trie_container : public hat_trie_node_base<alphabet_size, indexof> {
    friend class hat_trie<alphabet_size, indexof>;

  public:
    typedef array_hash store_type;

    hat_trie_container(char ch = '\0');
    virtual ~hat_trie_container() { }

    // accessors
    bool contains(const char *p) const;
    size_t size() const { return store.size(); }
    bool word() const { return _word; }

    // modifiers
    bool insert(const char *p);
    void set_word(bool b) { _word = b; }

  private:
    bool _word;
    array_hash store;
};

template <int alphabet_size, int (*indexof)(char)>
class hat_trie_node : public hat_trie_node_base<alphabet_size, indexof> {
    friend class hat_trie<alphabet_size, indexof>;

  private:
    typedef hat_trie_node_base<alphabet_size, indexof> node_base;

  public:
    hat_trie_node(char ch = '\0');
    ~hat_trie_node() { }

    // accessors
    bool word() const { return types[alphabet_size]; }

    // modifiers
    void set_word(bool b) { types[alphabet_size] = b; }

  private:
    std::bitset<alphabet_size + 1> types;  // extra bit is an end of word flag
    node_base *children[alphabet_size];  // untyped pointers to children
};

// ---------------------------------
// hat_trie_container implementation
// ---------------------------------

template <int alphabet_size, int (*indexof)(char)>
hat_trie_container<alphabet_size, indexof>::
hat_trie_container(char ch) :
        hat_trie_node_base<alphabet_size, indexof>(ch) {
    set_word(false);
}

template <int alphabet_size, int (*indexof)(char)>
bool hat_trie_container<alphabet_size, indexof>::
contains(const char *p) const {
    if (*p == '\0') {
        return word();
    }
    return store.find(p);
}

template <int alphabet_size, int (*indexof)(char)>
bool hat_trie_container<alphabet_size, indexof>::
insert(const char *p) {
    if (*p == '\0') {
        bool b = word();
        set_word(true);
        return !b;
    }
    return store.insert(p);
}

// ----------------------------
// hat_trie_node implementation
// ----------------------------

template <int alphabet_size, int (*indexof)(char)>
hat_trie_node<alphabet_size, indexof>::
hat_trie_node(char ch) :
        hat_trie_node_base<alphabet_size, indexof>(ch) {
    for (int i = 0; i < alphabet_size; ++i) {
        children[i] = NULL;
    }
}

}  // namespace stx

#endif // HAT_TRIE_NODE_H

