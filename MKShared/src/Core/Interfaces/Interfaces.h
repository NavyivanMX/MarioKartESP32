class IInitializable
{
public:
    virtual bool Initialize() noexcept = 0;

protected:
    ~IInitializable() = default;
};

class IUpdatable
{
public:
    virtual void Update() noexcept = 0;

protected:
    ~IUpdatable() = default;
};