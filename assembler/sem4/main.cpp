#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

struct Feature {
public:
  Feature() = default;
  explicit Feature(const std::string &feat) {
    std::size_t semicol_pos = feat.find(':');
    if (semicol_pos == std::string::npos)
      throw std::runtime_error("No : in feature");

    fnum = std::stoi(feat.substr(0, semicol_pos));
    val = std::stof(feat.substr(semicol_pos + 1));
  }

public:
  int fnum;
  float val;
};
using Case = std::vector<Feature>;

class Tree {
private:
  class Node {
  public:
    enum class Child { Left, Right };

    struct SplitNode {
      Feature pred;
      std::unique_ptr<Node> left;
      std::unique_ptr<Node> right;
    };

  public:
    explicit Node(float w) : content_(w) {}
    explicit Node(Feature pred) : content_(SplitNode{std::move(pred)}) {}

    Node(const Node &) = default;
    Node(Node &&) = default;
    Node &operator=(const Node &) = default;
    Node &operator=(Node &&) = default;

    Node *Append(Node n, Child pos) {
      assert(std::holds_alternative<SplitNode>(content_));
      if (pos == Child::Left) {
        std::get<SplitNode>(content_).left =
            std::make_unique<Node>(std::move(n));
        return std::get<SplitNode>(content_).left.get();
      } else {
        std::get<SplitNode>(content_).right =
            std::make_unique<Node>(std::move(n));
        return std::get<SplitNode>(content_).right.get();
      }
    }

    float Predict(const Case &) const;

  private:
    std::variant<SplitNode, float> content_;
  };

public:
  Tree(std::uint32_t D, std::istream &is);
  float Predict(const Case curr_case) const {
    return root_->Predict(curr_case);
  }

private:
  std::unique_ptr<Node> root_;
};

std::vector<Case> ReadCases(std::uint32_t M, std::uint32_t N, std::istream &is);

int main() {
  std::uint32_t N = 0;
  std::cin >> N;

  std::uint32_t T = 0, D = 0;
  std::cin >> T >> D;

  std::vector<Tree> model;
  for (std::uint32_t i = 0; i < T; i++)
    model.emplace_back(D, std::cin);

  std::uint32_t M = 0;
  std::cin >> M;
  std::vector<Case> cases = ReadCases(M, N, std::cin);

  for (auto it = cases.begin(); it != cases.end(); it++) {
    float result = 0.f;
    for (auto tree_it = model.begin(); tree_it != model.end(); tree_it++)
      result += tree_it->Predict(*it);
    std::cout << result << std::endl;
  }

  return 0;
}

Tree::Tree(std::uint32_t D, std::istream &is) {
  if (D == 1) { // Only one weight
    float w = 0;
    is >> w;

    root_ = std::make_unique<Node>(w);
    return;
  } else {
    std::string feature;
    is >> feature;
    root_ = std::make_unique<Node>(Feature(feature));
  }

  std::vector<Node *> curr_level, next_level;
  curr_level.push_back(root_.get());

  for (std::uint32_t curr_level_id = 2; curr_level_id <= D; curr_level_id++) {
    next_level.clear();

    for (auto it = curr_level.begin(); it != curr_level.end(); it++) {
      if (curr_level_id == D) { // Weights
        float wl = 0., wr = 0.;
        is >> wl >> wr;
        (*it)->Append(Node(wl), Node::Child::Left);
        (*it)->Append(Node(wr), Node::Child::Right);
      } else {
        std::string fleft, fright;
        is >> fleft >> fright;

        next_level.push_back(
            (*it)->Append(Node(Feature(fleft)), Node::Child::Left));
        next_level.push_back(
            (*it)->Append(Node(Feature(fright)), Node::Child::Right));
      }
    }

    std::swap(curr_level, next_level);
  }
}

/*
    Первая проблема: мы проходим эту *** рекурсией
*/
float Tree::Node::Predict(const Case &curr_case) const {
  if (std::holds_alternative<float>(content_))
    return std::get<float>(content_);
  else {
    const SplitNode &content_typed = std::get<SplitNode>(content_);
    assert(content_typed.left && content_typed.right);

    // float fval = 0.;
    // auto feature_it =
    //     std::find_if(
    //         curr_case.begin(), curr_case.end(),
    //         [pred = content_typed.pred](const Feature &f)
    //         {
    //             return f.fnum == pred.fnum;
    //         });
    auto feature = curr_case[content_typed.pred.fnum];
    float fval = feature.val;

    // if (feature_it != curr_case.end())
    //   fval = std::stof(feature_it->val);

    if (fval < content_typed.pred.val)
      return content_typed.left->Predict(curr_case);
    else
      return content_typed.right->Predict(curr_case);
  }
}

std::vector<Case> ReadCases(std::uint32_t M, std::uint32_t N,
                            std::istream &is) {
  {
    std::string dummy;
    std::getline(is, dummy); // read \n from input
  }

  std::vector<Case> cases;
  for (std::uint32_t i = 0; i < M; i++) {
    std::string case_str;
    std::getline(is, case_str);

    Case curr_case = std::vector<Feature>(N);
    std::stringstream case_stream(case_str);
    while (case_stream) {
      std::string feature;
      case_stream >> feature;
      if (feature.empty())
        break;

      auto f = Feature(feature);
      curr_case[f.fnum] = f;
    }

    cases.push_back(curr_case);
  }

  return cases;
}
