#include <iostream>
#include <queue>
#include <map>
#include <climits> // for CHAR_BIT
#include <iterator>
#include <algorithm>

const int UniqueSymbols = 1 << CHAR_BIT;
const char* SampleString = "Build me a son, O Lord, who will be strong enough to know when he is weak, and brave enough to face himself when he is afraid; one who will be proud and unbending in honest defeat, and humble and gentle in victory. Build me a son whose wishbone will not be where his backbone should be; a son who will know Thee and that to know himself is the foundation stone of knowledge. Lead him, I pray, not in the path of ease and comfort, but under the stress and spur of difficulties and challenge. Here let him learn to stand up in the storm; here let him learn compassion for those who fail. Build me a son whose heart will be clear, whose goal will be high; a son who will master himself before he seeks to master other men; one who will learn to laugh, yet never forget how to weep; one who will reach into the future, yet never forget the past. And after all these things are his, add, I pray, enough of a sense of humor, so that he may always be serious, yet never take himself too seriously. Give him humility, so that he may always remember the simplicity of true greatest, the open mind of true wisdom, the meekness of true strength. Then I, his father, will dare to whisper, I have not lived in vain!";

typedef std::vector<bool> HuffCode;
typedef std::map<char, HuffCode> HuffCodeMap;

class INode
{
public:
	const int f;

	virtual ~INode() {}

protected:
	INode(int f) : f(f) {}
};

class InternalNode : public INode
{
public:
	INode *const left;
	INode *const right;

	InternalNode(INode* c0, INode* c1) : INode(c0->f + c1->f), left(c0), right(c1) {}
	~InternalNode()
	{
		delete left;
		delete right;
	}
};

class LeafNode : public INode
{
public:
	const char c;

	LeafNode(int f, char c) : INode(f), c(c) {}
};

struct NodeCmp
{
	bool operator()(const INode* lhs, const INode* rhs) const { return lhs->f > rhs->f; }
};

INode* BuildTree(const int(&frequencies)[UniqueSymbols])
{
	std::priority_queue<INode*, std::vector<INode*>, NodeCmp> trees;

	for (int i = 0; i < UniqueSymbols; ++i)
	{
		if (frequencies[i] != 0)
			trees.push(new LeafNode(frequencies[i], (char)i));
	}
	while (trees.size() > 1)
	{
		INode* childR = trees.top();
		trees.pop();

		INode* childL = trees.top();
		trees.pop();

		INode* parent = new InternalNode(childR, childL);
		trees.push(parent);
	}
	return trees.top();
}

void GenerateCodes(const INode* node, const HuffCode& prefix, HuffCodeMap& outCodes)
{
	if (const LeafNode* lf = dynamic_cast<const LeafNode*>(node))
	{
		outCodes[lf->c] = prefix;
	}
	else if (const InternalNode* in = dynamic_cast<const InternalNode*>(node))
	{
		HuffCode leftPrefix = prefix;
		leftPrefix.push_back(false);
		GenerateCodes(in->left, leftPrefix, outCodes);

		HuffCode rightPrefix = prefix;
		rightPrefix.push_back(true);
		GenerateCodes(in->right, rightPrefix, outCodes);
	}
}

int main()
{
	// Build frequency table
	int frequencies[UniqueSymbols] = { 0 };
	const char* ptr = SampleString;
	while (*ptr != '\0')
		++frequencies[*ptr++];

	INode* root = BuildTree(frequencies);

	HuffCodeMap codes;
	GenerateCodes(root, HuffCode(), codes);
	delete root;

	for (HuffCodeMap::const_iterator it = codes.begin(); it != codes.end(); ++it)
	{
		std::cout << it->first << " ";
		std::copy(it->second.begin(), it->second.end(),
			std::ostream_iterator<bool>(std::cout));
		std::cout << std::endl;
	}
	system("pause");
	
}