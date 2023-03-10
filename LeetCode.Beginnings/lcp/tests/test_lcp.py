from lcp.lcp import get_lcp, get_lcp_v2


def test_get_lcp():
    strs = ['flower', 'flow', 'flair']
    assert get_lcp(strs) == 'fl'


def test_fail_get_lcp():
    strs = ['vorpal', 'vanquish', 'vain']
    assert get_lcp(strs) == 'v'


def test_fail_2_get_lcp():
    strs = ['cat', 'dog', 'vain']
    assert get_lcp(strs) == ''


def test_get_lcp_v2():
    strs = ['flower', 'flow', 'flair']
    assert get_lcp_v2(strs) == 'fl'
