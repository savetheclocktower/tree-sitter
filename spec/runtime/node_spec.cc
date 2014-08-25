#include "runtime/runtime_spec_helper.h"

extern "C" TSLanguage * ts_language_json();

START_TEST

describe("Node", []() {
  TSDocument *document;
  TSNode *root;

  before_each([&]() {
    document = ts_document_make();
    ts_document_set_language(document, ts_language_json());

    ts_document_set_input_string(document, "  [12, 5, 345]");
    root = ts_document_root_node(document);
    AssertThat(ts_node_string(root), Equals("(DOCUMENT (array (number) (number) (number)))"));
  });

  after_each([&]() {
    ts_document_free(document);
  });

  describe("child_count", [&]() {
    it("returns the number of visible child nodes", [&]() {
      TSNode *array = ts_node_child(root, 0);

      AssertThat(ts_node_child_count(array), Equals<size_t>(3));

      ts_node_release(array);
    });
  });

  describe("child(i)", [&]() {
    it("returns the child node at the given index", [&]() {
      TSNode *array = ts_node_child(root, 0);
      TSNode *number1 = ts_node_child(array, 0);
      TSNode *number2 = ts_node_child(array, 1);
      TSNode *number3 = ts_node_child(array, 2);

      AssertThat(ts_node_name(array), Equals("array"));
      AssertThat(ts_node_name(number1), Equals("number"));
      AssertThat(ts_node_name(number2), Equals("number"));
      AssertThat(ts_node_name(number3), Equals("number"));

      AssertThat(ts_node_pos(array), Equals<size_t>(2));
      AssertThat(ts_node_size(array), Equals<size_t>(12));

      AssertThat(ts_node_pos(number1), Equals<size_t>(3));
      AssertThat(ts_node_size(number1), Equals<size_t>(2));

      AssertThat(ts_node_pos(number2), Equals<size_t>(7));
      AssertThat(ts_node_size(number2), Equals<size_t>(1));

      AssertThat(ts_node_pos(number3), Equals<size_t>(10));
      AssertThat(ts_node_size(number3), Equals<size_t>(3));

      ts_node_release(array);
      ts_node_release(number1);
      ts_node_release(number2);
      ts_node_release(number3);
    });
  });

  describe("parent", [&]() {
    it("returns the node's parent node", [&]() {
      TSNode *array = ts_node_child(root, 0);
      TSNode *number = ts_node_child(array, 1);

      AssertThat(ts_node_parent(number), Equals(array));
      AssertThat(ts_node_parent(array), Equals(root));

      ts_node_release(array);
      ts_node_release(number);
    });
  });

  describe("next_sibling and prev_sibling", [&]() {
    it("returns the node's next and previous siblings", [&]() {
      TSNode *array = ts_node_child(root, 0);
      TSNode *number1 = ts_node_child(array, 0);
      TSNode *number2 = ts_node_child(array, 1);
      TSNode *number3 = ts_node_child(array, 2);

      AssertThat(ts_node_eq(ts_node_next_sibling(number2), number3), IsTrue());
      AssertThat(ts_node_eq(ts_node_prev_sibling(number2), number1), IsTrue());

      ts_node_release(array);
      ts_node_release(number1);
      ts_node_release(number2);
      ts_node_release(number3);
    });
  });

  describe("find_pos", [&]() {
    it("can retrieve the leaf node at a given position", [&]() {
      TSNode *number1 = ts_node_find_pos(root, 3);
      TSNode *number2 = ts_node_find_pos(root, 7);

      AssertThat(ts_node_name(number1), Equals("number"));
      AssertThat(ts_node_size(number1), Equals<size_t>(2));

      AssertThat(ts_node_name(number2), Equals("number"));
      AssertThat(ts_node_size(number2), Equals<size_t>(1));

      ts_node_release(number1);
      ts_node_release(number2);
    });

    it("returns higher-level nodes when no leaf is at the given position", [&]() {
      TSNode *node = ts_node_find_pos(root, 6);

      AssertThat(ts_node_name(node), Equals("array"));

      ts_node_release(node);
    });
  });
});

END_TEST
