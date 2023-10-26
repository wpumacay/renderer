import pytest

import renderer as rdr


def test_buffer_element_ctor_default() -> None:
    buff_elm = rdr.BufferElement()

    assert buff_elm.name == "none"
    assert buff_elm.type == rdr.ElementType.FLOAT_3
    assert buff_elm.count == 3
    assert buff_elm.size == 12
    assert buff_elm.offset == 0
    assert buff_elm.normalized is False


def test_buffer_element_ctor_args() -> None:
    elm_position = rdr.BufferElement("position", rdr.ElementType.FLOAT_3, False)
    elm_normal = rdr.BufferElement("normal", rdr.ElementType.FLOAT_3, True)
    elm_texcoord = rdr.BufferElement("texcoord", rdr.ElementType.FLOAT_2, False)

    assert elm_position.name == "position"
    assert elm_position.type == rdr.ElementType.FLOAT_3
    assert elm_position.count == 3
    assert elm_position.size == 12
    assert elm_position.offset == 0
    assert elm_position.normalized is False

    assert elm_normal.name == "normal"
    assert elm_normal.type == rdr.ElementType.FLOAT_3
    assert elm_normal.count == 3
    assert elm_normal.size == 12
    assert elm_normal.offset == 0
    assert elm_normal.normalized is True

    assert elm_texcoord.name == "texcoord"
    assert elm_texcoord.type == rdr.ElementType.FLOAT_2
    assert elm_texcoord.count == 2
    assert elm_texcoord.size == 8
    assert elm_texcoord.offset == 0
    assert elm_texcoord.normalized is False


@pytest.fixture
def layout():
    return rdr.BufferLayout(
        [
            ["position", rdr.ElementType.FLOAT_2, False],
            ["color", rdr.ElementType.FLOAT_3, False],
            ["texcoord", rdr.ElementType.FLOAT_2, False],
        ]
    )


def test_buffer_layout_ctor_default() -> None:
    layout = rdr.BufferLayout()

    assert len(layout) == 0
    assert layout.num_elements == 0
    assert layout.stride == 0


def test_buffer_layout_ctor_args(layout: rdr.BufferLayout) -> None:
    assert layout.num_elements == 3
    assert layout.stride == (2 * 4 + 3 * 4 + 2 * 4)


def test_buffer_layout_add_element_method() -> None:
    layout = rdr.BufferLayout()

    layout.AddElement(
        rdr.BufferElement("position", rdr.ElementType.FLOAT_3, False)
    )
    layout.AddElement(
        rdr.BufferElement("normal", rdr.ElementType.FLOAT_3, True)
    )
    layout.AddElement(
        rdr.BufferElement("texcoord", rdr.ElementType.FLOAT_2, False)
    )

    assert len(layout) == 3
    assert layout.num_elements == 3
    assert layout.stride == (3 * 4 + 3 * 4 + 2 * 4)


def test_buffer_layout_get_element_method(layout: rdr.BufferLayout) -> None:
    assert type(layout.GetElementByIndex(0)) is rdr.BufferElement
    assert type(layout.GetElementByIndex(1)) is rdr.BufferElement
    assert type(layout.GetElementByIndex(2)) is rdr.BufferElement

    assert layout.GetElementByIndex(0).name == "position"
    assert layout.GetElementByIndex(0).type == rdr.ElementType.FLOAT_2
    assert layout.GetElementByIndex(0).count == 2
    assert layout.GetElementByIndex(0).size == 2 * 4
    assert layout.GetElementByIndex(0).offset == 0
    assert layout.GetElementByIndex(0).normalized is False

    assert layout.GetElementByIndex(1).name == "color"
    assert layout.GetElementByIndex(1).type == rdr.ElementType.FLOAT_3
    assert layout.GetElementByIndex(1).count == 3
    assert layout.GetElementByIndex(1).size == 3 * 4
    assert layout.GetElementByIndex(1).offset == layout[0].size
    assert layout.GetElementByIndex(1).normalized is False

    assert layout.GetElementByIndex(2).name == "texcoord"
    assert layout.GetElementByIndex(2).type == rdr.ElementType.FLOAT_2
    assert layout.GetElementByIndex(2).count == 2
    assert layout.GetElementByIndex(2).size == 2 * 4
    assert layout.GetElementByIndex(2).offset == layout[0].size + layout[1].size
    assert layout.GetElementByIndex(2).normalized is False


def test_buffer_layout_getitem_operator(layout: rdr.BufferLayout) -> None:
    assert type(layout[0]) is rdr.BufferElement
    assert type(layout[1]) is rdr.BufferElement
    assert type(layout[2]) is rdr.BufferElement

    assert layout[0].name == "position"
    assert layout[0].type == rdr.ElementType.FLOAT_2
    assert layout[0].count == 2
    assert layout[0].size == 2 * 4
    assert layout[0].offset == 0
    assert layout[0].normalized is False

    assert layout[1].name == "color"
    assert layout[1].type == rdr.ElementType.FLOAT_3
    assert layout[1].count == 3
    assert layout[1].size == 3 * 4
    assert layout[1].offset == layout[0].size
    assert layout[1].normalized is False

    assert layout[2].name == "texcoord"
    assert layout[2].type == rdr.ElementType.FLOAT_2
    assert layout[2].count == 2
    assert layout[2].size == 2 * 4
    assert layout[2].offset == layout[0].size + layout[1].size
    assert layout[2].normalized is False
